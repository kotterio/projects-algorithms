from __future__ import annotations

import argparse
from datetime import datetime
import json
import multiprocessing as mp
from pathlib import Path
import time
from typing import Any, Callable

import numpy as np
import pandas as pd

from flowopt.pipeline import (
    REAL_CLEAN_FULL_ENRICHED_DATASET_PATH,
    run_real_gap_vrp,
    run_real_genetic,
    run_real_milp,
)


REPO_ROOT = Path(__file__).resolve().parents[2]
EXP_ROOT = Path(__file__).resolve().parent
DEFAULT_OUTPUT_ROOT = EXP_ROOT / "local" / "runs"


def _log(t0: float, msg: str) -> None:
    dt = time.perf_counter() - t0
    print(f"[+{dt:7.1f}s] {msg}", flush=True)


def _json_default(obj: Any) -> Any:
    if isinstance(obj, Path):
        return str(obj)
    try:
        if isinstance(obj, np.generic):
            return obj.item()
    except Exception:
        pass
    return str(obj)


def _parse_load_steps(raw: str) -> list[int]:
    values: list[int] = []
    for part in raw.split(","):
        s = part.strip()
        if not s:
            continue
        v = int(s)
        if v < 1 or v > 100:
            raise ValueError(f"Load value must be in [1..100], got: {v}")
        values.append(v)
    if not values:
        raise ValueError("No load steps provided.")
    return sorted(set(values))


def _agent_subset(agents: list[dict[str, Any]], agent_limit: int | None) -> list[dict[str, Any]]:
    if agent_limit is None or agent_limit <= 0 or agent_limit >= len(agents):
        return list(agents)
    # Stable deterministic subset.
    sorted_agents = sorted(agents, key=lambda a: str(a.get("agent_id", "")))
    return sorted_agents[:agent_limit]


def _build_subset_payload(
    *,
    base_payload: dict[str, Any],
    ordered_tasks: list[dict[str, Any]],
    n_tasks: int,
    load_pct: int,
    seed: int,
    agent_limit: int | None,
) -> dict[str, Any]:
    out = dict(base_payload)
    out["tasks"] = list(ordered_tasks[:n_tasks])
    out["routes"] = []
    out["agents"] = _agent_subset(list(base_payload.get("agents", [])), agent_limit)
    meta = dict(base_payload.get("metadata", {}))
    meta["exp5_load_profile"] = {
        "load_pct": load_pct,
        "tasks_selected": n_tasks,
        "tasks_base_total": len(base_payload.get("tasks", [])),
        "seed": seed,
        "agent_limit": agent_limit,
    }
    out["metadata"] = meta
    return out


def _write_dataset(path: Path, payload: dict[str, Any]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        json.dumps(payload, ensure_ascii=False, separators=(",", ":"), default=_json_default),
        encoding="utf-8",
    )


def _worker(func_name: str, kwargs: dict[str, Any], queue: mp.Queue) -> None:
    fn_map: dict[str, Callable[..., Any]] = {
        "real_gap_vrp": run_real_gap_vrp,
        "real_milp": run_real_milp,
        "real_genetic": run_real_genetic,
    }
    try:
        fn = fn_map[func_name]
        metrics = fn(**kwargs)
        data = metrics.as_dict()
        data.pop("agent_solution_rows", None)
        queue.put({"ok": True, "data": data})
    except Exception as exc:
        queue.put({"ok": False, "error": f"{type(exc).__name__}: {exc}"})


def _timeout_row(
    *,
    algorithm: str,
    dataset_path: str,
    tasks_total: int,
    agents_total: int,
    load_pct: int,
    elapsed: float,
    timeout_sec: int,
) -> dict[str, Any]:
    return {
        "algorithm": algorithm,
        "dataset_path": dataset_path,
        "load_pct": load_pct,
        "tasks_total": tasks_total,
        "agents_total": agents_total,
        "feasible": False,
        "all_checks_ok": False,
        "assigned_routes": 0,
        "unassigned_tasks": tasks_total,
        "active_agents": 0,
        "transport_work_ton_km": None,
        "total_km": None,
        "deadhead_km": None,
        "deadhead_share_pct": None,
        "total_hours": None,
        "runtime_sec": round(elapsed, 3),
        "solver_error": f"TimeoutError: exceeded timeout {timeout_sec}s",
    }


def _run_with_timeout(
    *,
    t0: float,
    algorithm: str,
    func_name: str,
    kwargs: dict[str, Any],
    dataset_path: Path,
    load_pct: int,
    tasks_total: int,
    agents_total: int,
    timeout_sec: int,
) -> dict[str, Any]:
    queue: mp.Queue = mp.Queue()
    p = mp.Process(target=_worker, args=(func_name, kwargs, queue), daemon=True)
    _log(t0, f"load={load_pct:3d}% | {algorithm}: START")
    w0 = time.perf_counter()
    p.start()
    p.join(timeout=timeout_sec)
    elapsed = time.perf_counter() - w0

    if p.is_alive():
        p.terminate()
        p.join(timeout=5)
        _log(t0, f"load={load_pct:3d}% | {algorithm}: TIMEOUT after {elapsed:.1f}s")
        return _timeout_row(
            algorithm=algorithm,
            dataset_path=str(dataset_path),
            tasks_total=tasks_total,
            agents_total=agents_total,
            load_pct=load_pct,
            elapsed=elapsed,
            timeout_sec=timeout_sec,
        )

    result: dict[str, Any] | None = None
    if not queue.empty():
        result = queue.get()

    if not result or not result.get("ok", False):
        err = "Worker failed without payload"
        if result and "error" in result:
            err = str(result["error"])
        _log(t0, f"load={load_pct:3d}% | {algorithm}: ERROR after {elapsed:.1f}s: {err}")
        row = _timeout_row(
            algorithm=algorithm,
            dataset_path=str(dataset_path),
            tasks_total=tasks_total,
            agents_total=agents_total,
            load_pct=load_pct,
            elapsed=elapsed,
            timeout_sec=timeout_sec,
        )
        row["solver_error"] = err
        return row

    row = dict(result["data"])
    row["load_pct"] = load_pct
    row["tasks_total"] = tasks_total
    row["agents_total"] = agents_total
    row["runtime_sec"] = round(elapsed, 3)
    if tasks_total > 0 and row.get("unassigned_tasks") is not None:
        row["assignment_coverage_pct"] = round(
            100.0 * (tasks_total - int(row["unassigned_tasks"])) / tasks_total,
            3,
        )
    else:
        row["assignment_coverage_pct"] = None
    _log(t0, f"load={load_pct:3d}% | {algorithm}: DONE in {elapsed:.1f}s")
    return row


def _method_specs(args: argparse.Namespace, dataset_path: Path) -> list[tuple[str, str, dict[str, Any]]]:
    return [
        (
            "real_gap_vrp",
            "real_gap_vrp",
            {
                "dataset_path": dataset_path,
                "step1_method": args.gap_step1_method,
                "gap_iter": args.gap_iter,
                "use_repair": True,
                "show_progress": False,
                "verbose": False,
            },
        ),
        (
            "real_milp",
            "real_milp",
            {
                "dataset_path": dataset_path,
                "time_limit_sec": args.milp_time_limit_sec,
                "unassigned_penalty": args.milp_unassigned_penalty,
                "show_progress": False,
            },
        ),
        (
            "real_genetic",
            "real_genetic",
            {
                "dataset_path": dataset_path,
                "population_size": args.genetic_population,
                "generations": args.genetic_generations,
                "generation_scale": args.genetic_generation_scale,
                "elite_size": args.genetic_elite,
                "seed": args.seed,
                "show_progress": False,
            },
        ),
    ]


def _write_load_report(load_dir: Path, rows: list[dict[str, Any]]) -> None:
    df = pd.DataFrame(rows)
    cols = [
        "algorithm",
        "feasible",
        "all_checks_ok",
        "assigned_routes",
        "unassigned_tasks",
        "assignment_coverage_pct",
        "active_agents",
        "transport_work_ton_km",
        "total_km",
        "deadhead_share_pct",
        "total_hours",
        "runtime_sec",
        "solver_error",
    ]
    for c in cols:
        if c not in df.columns:
            df[c] = None
    md = []
    md.append(f"# EXP5 load report: `{load_dir.name}`")
    md.append("")
    md.append(df[cols].to_markdown(index=False))
    md.append("")
    load_dir.joinpath("README.md").write_text("\n".join(md), encoding="utf-8")
    df.to_csv(load_dir / "results.csv", index=False)
    (load_dir / "results.json").write_text(
        json.dumps(rows, ensure_ascii=False, indent=2, default=_json_default),
        encoding="utf-8",
    )


def _write_global_report(
    *,
    out_root: Path,
    rows: list[dict[str, Any]],
    generated_at: str,
) -> tuple[Path, Path, Path]:
    out_root.parent.mkdir(parents=True, exist_ok=True)
    stem = f"exp5_load_sweep_{generated_at}"
    p_json = out_root.parent / f"{stem}.json"
    p_csv = out_root.parent / f"{stem}.csv"
    p_md = out_root.parent / f"{stem}.md"
    p_json.write_text(json.dumps(rows, ensure_ascii=False, indent=2, default=_json_default), encoding="utf-8")

    df = pd.DataFrame(rows)
    df.to_csv(p_csv, index=False)

    cols = [
        "load_pct",
        "algorithm",
        "feasible",
        "all_checks_ok",
        "assigned_routes",
        "unassigned_tasks",
        "assignment_coverage_pct",
        "active_agents",
        "runtime_sec",
        "solver_error",
    ]
    for c in cols:
        if c not in df.columns:
            df[c] = None

    lines: list[str] = []
    lines.append("# EXP5 Report: Incremental Load Sweep")
    lines.append("")
    lines.append(f"Generated: {datetime.now().isoformat(timespec='seconds')}")
    lines.append("")

    for load_pct in sorted(df["load_pct"].dropna().unique().tolist()):
        lines.append(f"## Load `{int(load_pct)}%`")
        lines.append("")
        part = df[df["load_pct"] == load_pct][cols].copy()
        lines.append(part.to_markdown(index=False))
        lines.append("")

    lines.append("## Aggregate by algorithm")
    lines.append("")
    pivot = (
        df.groupby("algorithm", dropna=False)
        .agg(
            runs=("algorithm", "count"),
            feasible_runs=("feasible", lambda s: int(pd.Series(s).fillna(False).sum())),
            avg_runtime_sec=("runtime_sec", "mean"),
            avg_coverage_pct=("assignment_coverage_pct", "mean"),
        )
        .reset_index()
    )
    lines.append(pivot.to_markdown(index=False))
    lines.append("")

    p_md.write_text("\n".join(lines), encoding="utf-8")
    return p_json, p_csv, p_md


def main() -> int:
    parser = argparse.ArgumentParser(description="EXP5: load sweep on clean_full_enriched dataset")
    parser.add_argument(
        "--dataset-path",
        type=Path,
        default=REAL_CLEAN_FULL_ENRICHED_DATASET_PATH,
        help="Path to base clean_full_enriched dataset JSON",
    )
    parser.add_argument(
        "--load-steps",
        type=str,
        default="10,20,30,40,50,60,70,80,90,100",
        help="Comma-separated load percents in [1..100]",
    )
    parser.add_argument(
        "--agent-limit",
        type=int,
        default=96,
        help="Optional hard limit on number of agents in generated sub-datasets (<=0 means all)",
    )
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--timeout-sec", type=int, default=600)
    parser.add_argument(
        "--algorithms",
        nargs="+",
        default=["real_gap_vrp", "real_milp", "real_genetic"],
        choices=["real_gap_vrp", "real_milp", "real_genetic"],
    )
    parser.add_argument("--gap-step1-method", type=str, default="dataset", choices=["dataset", "greedy", "lp"])
    parser.add_argument("--gap-iter", type=int, default=2)
    parser.add_argument("--milp-time-limit-sec", type=int, default=45)
    parser.add_argument("--milp-unassigned-penalty", type=float, default=1e5)
    parser.add_argument("--genetic-population", type=int, default=12)
    parser.add_argument("--genetic-generations", type=int, default=20)
    parser.add_argument("--genetic-generation-scale", type=float, default=0.1)
    parser.add_argument("--genetic-elite", type=int, default=3)
    parser.add_argument("--output-root", type=Path, default=DEFAULT_OUTPUT_ROOT)
    parser.add_argument("--force", action="store_true", help="Re-run and overwrite existing per-load reports")
    args = parser.parse_args()

    t0 = time.perf_counter()
    dataset_path = args.dataset_path.resolve()
    if not dataset_path.exists():
        raise FileNotFoundError(f"Dataset not found: {dataset_path}")

    _log(t0, f"dataset: {dataset_path}")
    base_payload = json.loads(dataset_path.read_text(encoding="utf-8"))
    tasks_all = list(base_payload.get("tasks", []))
    total_tasks = len(tasks_all)
    if total_tasks == 0:
        raise RuntimeError("Base dataset has zero tasks.")

    load_steps = _parse_load_steps(args.load_steps)
    _log(t0, f"load steps: {load_steps}")
    rng = np.random.default_rng(args.seed)
    perm = rng.permutation(total_tasks)
    ordered_tasks = [tasks_all[int(i)] for i in perm.tolist()]
    _log(t0, f"task permutation prepared: total={total_tasks}")

    out_root = args.output_root.resolve()
    out_root.mkdir(parents=True, exist_ok=True)
    all_rows: list[dict[str, Any]] = []

    for load_pct in load_steps:
        n_tasks = max(1, int(round(total_tasks * load_pct / 100.0)))
        load_dir = out_root / f"load_{load_pct:03d}"
        load_dir.mkdir(parents=True, exist_ok=True)
        dataset_out = load_dir / "dataset.json"
        _log(t0, f"prepare load={load_pct}% -> tasks={n_tasks}")

        subset_payload = _build_subset_payload(
            base_payload=base_payload,
            ordered_tasks=ordered_tasks,
            n_tasks=n_tasks,
            load_pct=load_pct,
            seed=args.seed,
            agent_limit=(args.agent_limit if args.agent_limit > 0 else None),
        )
        _write_dataset(dataset_out, subset_payload)
        agents_total = len(subset_payload.get("agents", []))

        load_rows: list[dict[str, Any]] = []
        if not args.force and (load_dir / "results.json").exists():
            _log(t0, f"load={load_pct}% already has results.json; recompute skipped (use --force)")
            try:
                prev = json.loads((load_dir / "results.json").read_text(encoding="utf-8"))
                if isinstance(prev, list):
                    load_rows = [dict(x) for x in prev]
                    all_rows.extend(load_rows)
                    continue
            except Exception:
                pass

        specs = _method_specs(args, dataset_out)
        wanted = set(args.algorithms)
        for algorithm, func_name, kwargs in specs:
            if algorithm not in wanted:
                continue
            row = _run_with_timeout(
                t0=t0,
                algorithm=algorithm,
                func_name=func_name,
                kwargs=kwargs,
                dataset_path=dataset_out,
                load_pct=load_pct,
                tasks_total=n_tasks,
                agents_total=agents_total,
                timeout_sec=args.timeout_sec,
            )
            load_rows.append(row)
            all_rows.append(row)

        _write_load_report(load_dir, load_rows)
        _log(t0, f"load={load_pct}% report saved: {load_dir}")

    ts = datetime.now().strftime("%Y%m%d_%H%M%S")
    p_json, p_csv, p_md = _write_global_report(out_root=out_root, rows=all_rows, generated_at=ts)
    _log(t0, f"global report json: {p_json}")
    _log(t0, f"global report csv : {p_csv}")
    _log(t0, f"global report md  : {p_md}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

