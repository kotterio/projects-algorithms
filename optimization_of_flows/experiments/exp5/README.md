# EXP5: Incremental Load Sweep (`10%..100%`)

Постепенно увеличиваем количество задач (кумулятивно по одной и той же перестановке задач)
и запускаем 3 базовых метода:

- `real_gap_vrp`
- `real_milp`
- `real_genetic`

База: `src/data/real/clean_full_enriched/dataset_real_spb_clean_full_enriched.json`.

Для практической выполнимости в этом эксперименте используется ограничение на размер флота
(`--agent-limit`, по умолчанию `96`) и жесткий timeout на запуск метода.

## Запуск

```bash
PYTHONPATH=src python experiments/exp5/run_exp5_load_sweep.py \
  --load-steps 10,20,30,40,50,60,70,80,90,100 \
  --agent-limit 96 \
  --timeout-sec 600
```

Быстрый прогон:

```bash
PYTHONPATH=src python experiments/exp5/run_exp5_load_sweep.py \
  --load-steps 10,20,30 \
  --agent-limit 64 \
  --timeout-sec 180 \
  --gap-iter 1 \
  --milp-time-limit-sec 30 \
  --genetic-generations 12 \
  --genetic-generation-scale 0.1
```

## Артефакты

- Пер-нагрузочные папки: `experiments/exp5/local/runs/load_XXX/`
  - `dataset.json`
  - `results.json`
  - `results.csv`
  - `README.md`
- Глобальный отчет:
  - `experiments/exp5/local/exp5_load_sweep_*.json`
  - `experiments/exp5/local/exp5_load_sweep_*.csv`
  - `experiments/exp5/local/exp5_load_sweep_*.md`

