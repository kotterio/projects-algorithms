# 🚚 Optimization of Flows

This project compares different algorithms for optimizing transportation flows and routing in urban logistics.

We evaluate three approaches:

* **MILP (Mixed Integer Linear Programming)** — exact optimization
* **GAP + VRP heuristic** — decomposition approach
* **Genetic Algorithm (GA)** — metaheuristic method

The comparison is performed on a synthetic dataset representing transportation tasks in an urban environment.

---

## 📂 Project Structure

### 🔹 `First_step_synthetic_data/`

Contains the **MILP-based solver** for the transportation problem.

**Main files:**

* `simple_solver_milp.py` — core MILP model using linear optimization
* `simple_solver_components.py` — helper functions and cost calculations

👉 Produces **optimal routing solution** with minimal transport work.

---

### 🔹 `genetic_algo_synthetic_data/`

Implementation of the **Genetic Algorithm solver**.

**Main files:**

* `genetic_solver_min.py` — main GA runner (multi-day simulation)
* `genetic_solver_components_improved.py` — GA operators (mutation, crossover, evaluation)

👉 Produces **near-optimal solutions** using evolutionary optimization.

---

### 🔹 `synthetic_data_gap_vrp_solver/`

Contains the **GAP + VRP decomposition solver**.

**Main files:**

* `gap_vrp_solver.py` — main logic:

  * task assignment (GAP)
  * routing heuristics (VRP components)
* `dataset.py` — data structures (graph, tasks, agents, routes)

👉 First assigns tasks to agents, then builds routes heuristically.

---

### 🔹 Root files

* `dataset_sandbox_type2.json` — synthetic dataset:

  * graph (road network)
  * agents (vehicles)
  * tasks (transport demands)
  * metadata (depots)

* `README.md` — project description

---

## ⚙️ Methods Overview

### 🧮 MILP

* Guarantees optimal solution
* Minimizes total transport work (ton-km)
* Computationally expensive

---

### 🧩 GAP + VRP

* Step 1: assign tasks to agents (GAP)
* Step 2: build routes (VRP heuristics)
* Faster but approximate

---

### 🧬 Genetic Algorithm

* Population-based search
* Uses mutation and crossover
* Flexible and scalable

---

## 📊 Metrics

All methods are compared using:

* `assigned_routes` — number of completed routes
* `unassigned_tasks` — tasks not served
* `active_agents` — number of used vehicles
* `transport_work_ton_km` — main efficiency metric

---

## 🗺️ Visualization

Routes can be visualized on a map (e.g., Saint Petersburg) using `folium`.

* MILP → exact routes
* GA → heuristic routes
* GAP → assignment only (no full routing)

---

## 🚀 How to Run (Colab)

1. Upload all files to `/content`
2. Run:

   * MILP solver
   * GAP + VRP solver
   * Genetic solver
3. Compare results
4. Visualize routes on map

---

## 🧠 Conclusion

* **MILP** provides the best solution quality
* **Genetic Algorithm** gives competitive results with flexibility
* **GAP + VRP** is fast and scalable, but less precise

---

## 📌 Future Improvements

* Full VRP integration for GAP results
* Runtime comparison
* Multi-day simulation analysis
* Real-world datasets

---

## 👩‍💻 Authors

* Igor Ignashin
* Anna Komleva
* Kristina Abgaryan
* Ksenia Vydrina
