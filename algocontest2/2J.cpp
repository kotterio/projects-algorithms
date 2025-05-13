#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using Graph = std::vector<std::vector<int>>;

bool TopSortedDfs(const Graph&, std::vector<uint8_t>&, size_t, std::vector<size_t>&);

std::vector<size_t> TopSort(const Graph& g) {
  std::vector<uint8_t> color(g.size(), 0);
  std::vector<size_t> topsorted;
  topsorted.reserve(g.size());
  for (size_t i = 0; i < g.size(); i++) {
    if (color[i] == 0) {
      if (!TopSortedDfs(g, color, i, topsorted)) {
        std::vector<size_t> resh;
        return resh;
      }
    }
  }
  std::reverse(topsorted.begin(), topsorted.end());
  return topsorted;
}

bool TopSortedDfs(const Graph& g, std::vector<uint8_t>& color, size_t v, std::vector<size_t>& topsorted) {
  color[v] = 1;
  for (size_t u : g[v]) {
    if (color[u] == 1) {
      return false;
    }
    if (color[u] == 0) {
      if (!TopSortedDfs(g, color, u, topsorted)) {
        return false;
      }
    }
  }
  color[v] = 2;
  topsorted.push_back(v + 1);
  return true;
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
    g[from].emplace_back(to);
  }
  std::vector<size_t> result = TopSort(g);
  if (result.empty()) {
    std::cout << -1 << std::endl;
    return 0;
  }
  for (size_t i = 0; i < result.size(); i++) {
    std::cout << result[i] << " ";
  }
  return 0;
}