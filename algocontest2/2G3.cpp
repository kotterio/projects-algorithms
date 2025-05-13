#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

using Graph = std::vector<std::vector<size_t>>;
bool HasCycleDfs(const Graph&, size_t, std::vector<uint8_t>&, std::vector<size_t>&, std::vector<size_t>&);
void HasCycle(const Graph& g, std::vector<size_t>& parent) {
  std::vector<uint8_t> color(g.size(), 0);
  std::vector<size_t> res(g.size());
  for (size_t j = 0; j < g.size(); ++j) {
    if (color[j] == 0) {
      parent[j] = j;
      if (HasCycleDfs(g, j, color, parent, res)) {
        std::reverse(res.begin(), res.end());
        std::cout << "YES" << std::endl;
        for (size_t k = 0; k < res.size(); k++) {
          if (res[k] != 0) {
            std::cout << res[k] << std::endl;
          }
        }
        return;
      }
    }
  }
  std::cout << "NO";
}
bool HasCycleDfs(const Graph& g, size_t v, std::vector<uint8_t>& color, std::vector<size_t>& parent,
                 std::vector<size_t>& res) {
  color[v] = 1;
  bool vg = false;
  for (size_t i = 0; i < g[v].size(); ++i) {
    size_t u = g[v][i];
    if (color[u] == 1) {
      parent[u] = v;
      size_t k = v;
      size_t o = 0;
      while (k != u) {
        res[o] = k + 1;
        k = parent[k];
        ++o;
      }
      res[o] = u + 1;
      return true;
    }
    if (color[u] == 0) {
      parent[u] = v;
      if (HasCycleDfs(g, u, color, parent, res)) {
        vg = true;
        break;
      }
    }
  }
  color[v] = 2;
  return vg;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  size_t v = 0;
  size_t e = 0;
  std::cin >> v >> e;
  Graph g(v);
  for (size_t i = 0; i < e; ++i) {
    size_t to = 0;
    size_t from = 0;
    std::cin >> from >> to;
    --to;
    --from;
    if (from != to) {
      g[from].emplace_back(to);
    }
  }
  std::vector<size_t> parent(g.size(), -1);
  HasCycle(g, parent);
}