#include <limits>
#include <vector>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <set>

using Graph = std::vector<std::vector<size_t>>;
void DFSvisit(const Graph&, size_t, bool, std::vector<uint8_t>&, std::vector<int>&, std::vector<int>&, int&,
              std::set<size_t>&);
std::set<size_t> DFS(const Graph& g) {
  int inf = 200000000;
  std::vector<uint8_t> color(g.size(), 0);
  std::vector<int> time_in(g.size(), inf);
  std::vector<int> time_up(g.size(), inf);
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
        articulation_points.insert(v);
      }
    }
  }
  if (is_root && n_child > 1) {
    articulation_points.insert(v);
  }
  color[v] = 2;
}

int main() {
  size_t v = 0;
  size_t e = 0;
  std::cin >> v >> e;
  Graph g(v + e);
  for (size_t i = 0; i < e; i++) {
    size_t one = 0;
    size_t two = 0;
    size_t three = 0;
    std::cin >> one >> two >> three;
    --one;
    --two;
    --three;
    g[one].emplace_back(i + v);
    g[two].emplace_back(i + v);
    g[three].emplace_back(i + v);
    g[i + v].emplace_back(one);
    g[i + v].emplace_back(two);
    g[i + v].emplace_back(three);
  }
  std::set<size_t> result = DFS(g);
  std::set<size_t> pillow;
  for (auto elem : result) {
    if (elem >= v) {
      pillow.insert(elem - v + 1);
    }
  }
  std::cout << pillow.size() << std::endl;
  for (auto elem : pillow) {
    std::cout << elem << std::endl;
  }
}