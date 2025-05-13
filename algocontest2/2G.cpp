#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <unordered_set>

using Graph = std::vector<std::unordered_set<int64_t>>;

bool HasCycleDfs(const Graph&, int64_t, std::vector<uint8_t>&, int64_t);
bool HasCycle(const Graph& g) {
  std::vector<uint8_t> color(g.size(), 0);
  if (HasCycleDfs(g, 0, color, -1)) {
    return true;
  }
  for (size_t j = 0; j < g.size(); ++j) {
    if (color[j] == 0) {
      return true;
    }
  }
  return false;
}
bool HasCycleDfs(const Graph& g, int64_t v, std::vector<uint8_t>& color, int64_t parent) {
  color[v] = 1;
  for (auto& u : g[v]) {
    if (color[u] == 1 && u != parent) {
      return true;
    }
    if (color[u] == 0) {
      if (HasCycleDfs(g, u, color, v)) {
        return true;
      }
    }
  }
  color[v] = 2;
  return false;
}

std::pair<bool, bool> Alkan(Graph& g, size_t m, std::vector<bool>& id) {
  bool ident1 = false;
  bool ident2 = false;
  char atom = 'O';
  std::vector<size_t> num(g.size(), 0);
  for (size_t i = 0; i < g.size(); i++) {
    std::cin >> atom;
    if (atom == 'C') {
      ident1 = true;
      id[i] = true;
    } else {
      ident2 = true;
    }
  }
  for (size_t i = 0; i < m; i++) {
    int64_t start = 0;
    int64_t end = 0;
    std::cin >> start >> end;
    --start;
    --end;
    if (start == end) {
      ident2 = false;
    }
    ++num[start];
    ++num[end];
    g[start].insert(end);
    g[end].insert(start);
  }
  for (size_t i = 0; i < g.size(); i++) {
    if (g[i].size() != num[i]) {
      ident1 = false;
    }
  }
  std::pair<bool, bool> res = {ident1, ident2};
  return res;
}
int main() {
  int64_t v = 0;
  int64_t e = 0;
  std::cin >> v >> e;
  Graph g(v);
  std::vector<bool> id(g.size(), false);
  auto res = Alkan(g, e, id);
  if (v < 5 || e < 4 || HasCycle(g) || !res.first || !res.second) {
    std::cout << "NO";
    return 0;
  }
  for (size_t i = 0; i < g.size(); i++) {
    if (id[i]) {
      if (g[i].size() != 4) {
        std::cout << "NO";
        return 0;
      }
    } else if (g[i].size() != 1 || !id[*g[i].begin()]) {
      std::cout << "NO";
      return 0;
    }
  }
  std::cout << "YES";
  return 0;
}