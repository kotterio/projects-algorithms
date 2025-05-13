#include <vector>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <set>

using Graph = std::vector<std::vector<size_t>>;
void DFSvisit(const Graph&, size_t, bool, std::vector<uint8_t>&, std::vector<int>&, std::vector<int>&, int&,
              std::set<size_t>&);
std::set<size_t> DFS(const Graph& g) {
  std::vector<uint8_t> color(g.size(), 0);
  std::vector<int> time_in(g.size());
  std::vector<int> time_up(g.size());
  std::set<size_t> articulation_points;
  int time = 0;
  for (size_t i = 0; i < g.size(); i++) {
    if (color[i] == 0) {
      DFSvisit(g, i, true, color, time_in, time_up, time, articulation_points);
    }
  }
  return articulation_points;
}
void DFSvisit(const Graph& g, size_t v, bool is_root, std::vector<uint8_t>& color, std::vector<int>& time_in,
              std::vector<int>& time_up, int& time, std::set<size_t>& articulation_points) {
  color[v] = 1;
  time_in[v] = time_up[v] = ++time;
  size_t n_child = 0;
  for (const auto u : g[v]) {
    if (color[u] == 1) {
      time_up[v] = std::min(time_in[u], time_up[v]);
    }
    if (color[u] == 0) {
      ++n_child;
      DFSvisit(g, u, false, color, time_in, time_up, time, articulation_points);
      time_up[v] = std::min(time_up[v], time_up[u]);
      if (!is_root && time_in[v] <= time_up[u]) {
        articulation_points.insert(v + 1);
      }
    }
  }
  if (is_root && n_child > 1) {
    articulation_points.insert(v + 1);
  }
  color[v] = 2;
}

int main() {
  size_t v = 0;
  size_t e = 0;
  std::cin >> v >> e;
  Graph g(v);
  for (size_t i = 0; i < e; i++) {
    size_t to = 0;
    size_t from = 0;
    std::cin >> from >> to;
    --to;
    --from;
    g[from].emplace_back(to);
    g[to].emplace_back(from);
  }
  std::set<size_t> result = DFS(g);
  std::cout << result.size() << std::endl;
  for (auto elem : result) {
    std::cout << elem << std::endl;
  }
}