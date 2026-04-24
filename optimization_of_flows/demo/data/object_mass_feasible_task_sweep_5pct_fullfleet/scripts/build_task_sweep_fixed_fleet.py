#!/usr/bin/env python3
from __future__ import annotations

import argparse
import copy
import csv
import json
import math
import random
from collections import defaultdict
from pathlib import Path
from typing import Any


def _load_json(path: Path) -> dict[str, Any]:
    return json.loads(path.read_text(encoding="utf-8"))


def _save_json(path: Path, payload: dict[str, Any]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(payload, ensure_ascii=False, indent=2), encoding="utf-8")


def _stratified_order(
    task_ids: list[str],
    task_xy: dict[str, tuple[float, float]],
    grid_size: int,
    seed: int,
) -> list[str]:
    rng = random.Random(seed)
    if not task_ids:
        return []

    xs = [task_xy[tid][0] for tid in task_ids]
    ys = [task_xy[tid][1] for tid in task_ids]
    min_x, max_x = min(xs), max(xs)
    min_y, max_y = min(ys), max(ys)
    span_x = max(max_x - min_x, 1e-9)
    span_y = max(max_y - min_y, 1e-9)

    buckets: dict[tuple[int, int], list[str]] = defaultdict(list)
    for tid in task_ids:
        x, y = task_xy[tid]
        cx = min(grid_size - 1, max(0, int((x - min_x) / span_x * grid_size)))
        cy = min(grid_size - 1, max(0, int((y - min_y) / span_y * grid_size)))
        buckets[(cx, cy)].append(tid)

    keys = list(buckets.keys())
    for key in keys:
        rng.shuffle(buckets[key])

    ordered: list[str] = []
    while True:
        progressed = False
        rng.shuffle(keys)
        for key in keys:
            if not buckets[key]:
                continue
            ordered.append(buckets[key].pop())
            progressed = True
        if not progressed:
            break

    return ordered


def _nearest_depot(task: dict[str, Any], node_by_id: dict[str, dict[str, Any]], depot_ids: list[str]) -> str:
    src = str(task["source_node_id"])
    src_node = node_by_id.get(src)
    if src_node is None:
        return depot_ids[0]
    sx, sy = float(src_node["x"]), float(src_node["y"])
    best_dep = depot_ids[0]
    best_d2 = float("inf")
    for dep in depot_ids:
        dn = node_by_id.get(dep)
        if dn is None:
            continue
        dx = float(dn["x"]) - sx
        dy = float(dn["y"]) - sy
        d2 = dx * dx + dy * dy
        if d2 < best_d2:
            best_d2 = d2
            best_dep = dep
    return best_dep


def main() -> None:
    here = Path(__file__).resolve().parent
    default_source = here.parent / "data" / "day_object_mass_feasible" / "dataset_real_spb_day_object_mass_feasible.json"
    default_out = here.parent / "data" / "task_sweep_object_mass_feasible_5pct"

    parser = argparse.ArgumentParser(description="Build task-load sweep from source dataset with fixed fleet.")
    parser.add_argument("--source", type=Path, default=default_source)
    parser.add_argument("--out-dir", type=Path, default=default_out)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--grid-size", type=int, default=6)
    parser.add_argument("--start-pct", type=int, default=5)
    parser.add_argument("--stop-pct", type=int, default=100)
    parser.add_argument("--step-pct", type=int, default=5)
    parser.add_argument("--name-prefix", type=str, default="real_spb_day_object_mass_feasible")
    args = parser.parse_args()

    data = _load_json(args.source)
    nodes = data.get("graph", {}).get("nodes", [])
    agents = data.get("agents", [])
    tasks = data.get("tasks", [])
    meta = data.get("metadata", {})

    if not nodes or not agents or not tasks:
        raise ValueError("source dataset must contain graph nodes, agents and tasks")

    node_by_id = {str(n["node_id"]): n for n in nodes}
    depot_ids = [str(x) for x in meta.get("depot_node_ids", [])]
    if not depot_ids:
        depot_ids = sorted({str(v) for v in meta.get("agent_depots", {}).values() if v is not None})
    if not depot_ids:
        raise ValueError("No depot ids found in metadata")

    task_xy: dict[str, tuple[float, float]] = {}
    for t in tasks:
        tid = str(t["task_id"])
        src = str(t["source_node_id"])
        n = node_by_id.get(src)
        if n is None:
            continue
        task_xy[tid] = (float(n["x"]), float(n["y"]))

    task_ids = [str(t["task_id"]) for t in tasks if str(t["task_id"]) in task_xy]
    ordered_ids = _stratified_order(task_ids, task_xy, args.grid_size, args.seed)
    rank = {tid: i for i, tid in enumerate(ordered_ids)}
    tasks_sorted = sorted((t for t in tasks if str(t["task_id"]) in rank), key=lambda t: rank[str(t["task_id"])])

    args.out_dir.mkdir(parents=True, exist_ok=True)
    profiles: list[dict[str, Any]] = []
    total_tasks = len(tasks_sorted)

    for pct in range(args.start_pct, args.stop_pct + 1, args.step_pct):
        target_n = int(math.ceil(total_tasks * pct / 100.0))
        target_n = max(1, min(total_tasks, target_n))
        selected_tasks = tasks_sorted[:target_n]

        selected_sources = {str(t["source_node_id"]) for t in selected_tasks}
        selected_dests = {str(t["destination_node_id"]) for t in selected_tasks}

        tasks_by_depot = {dep: 0 for dep in depot_ids}
        for t in selected_tasks:
            dep = _nearest_depot(t, node_by_id, depot_ids)
            tasks_by_depot[dep] = tasks_by_depot.get(dep, 0) + 1

        payload = {
            "metadata": copy.deepcopy(data.get("metadata", {})),
            "graph": data.get("graph", {}),
            "agents": data.get("agents", []),
            "tasks": selected_tasks,
            "routes": [],
            "derived": data.get("derived", {}),
        }

        tag = f"p{pct:03d}"
        ds_name = f"dataset_{args.name_prefix}_tasks_{tag}.json"
        sm_name = f"summary_{args.name_prefix}_tasks_{tag}.json"

        md = payload.setdefault("metadata", {})
        md["name"] = f"{args.name_prefix}_tasks_{tag}"
        md["task_load_profile"] = {
            "source_dataset": str(args.source),
            "selection": "spatial_stratified_prefix",
            "seed": args.seed,
            "grid_size": args.grid_size,
            "task_percent": pct,
            "selected_tasks": target_n,
            "total_source_tasks": total_tasks,
            "tasks_by_nearest_depot": tasks_by_depot,
        }
        md.setdefault("counts", {})
        md["counts"].update(
            {
                "tasks": int(target_n),
                "mno": int(len(selected_sources)),
                "object1": int(len(selected_dests)),
                "agents": int(len(agents)),
                "depots": int(len(set(md.get("agent_depots", {}).values()))),
                "routes": 0,
            }
        )

        ds_path = args.out_dir / ds_name
        sm_path = args.out_dir / sm_name
        _save_json(ds_path, payload)
        _save_json(
            sm_path,
            {
                "dataset_file": ds_name,
                "source_dataset": str(args.source),
                "profile": md["task_load_profile"],
                "counts": md["counts"],
            },
        )

        profiles.append(
            {
                "task_percent": pct,
                "dataset": ds_name,
                "summary": sm_name,
                "tasks": target_n,
                "agents": len(agents),
                "mno": len(selected_sources),
                "objects": len(selected_dests),
            }
        )

    index_payload = {
        "source_dataset": str(args.source),
        "notes": "Task-load sweep with fixed full fleet.",
        "selection": {
            "method": "spatial_stratified_prefix",
            "seed": args.seed,
            "grid_size": args.grid_size,
            "percents": list(range(args.start_pct, args.stop_pct + 1, args.step_pct)),
        },
        "profiles": profiles,
    }
    _save_json(args.out_dir / "index_task_load_sweep_5pct.json", index_payload)

    csv_path = args.out_dir / "task_load_sweep_5pct.csv"
    with csv_path.open("w", encoding="utf-8", newline="") as f:
        writer = csv.DictWriter(
            f,
            fieldnames=["task_percent", "tasks", "agents", "mno", "objects", "dataset", "summary"],
        )
        writer.writeheader()
        for row in profiles:
            writer.writerow({k: row[k] for k in writer.fieldnames})

    print(json.dumps(index_payload, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
