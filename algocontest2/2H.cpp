#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using Graph = std::vector<size_t>;
bool HasCycleDfs(const Graph&, size_t, std::vector<uint8_t>&);
size_t HasCycle(const Graph& g) {
  std::vector<uint8_t> color(g.size(), 0);
  size_t i = 0;
  for (size_t j = 0; j < g.size(); j++) {
    if (color[j] == 0) {
      if (HasCycleDfs(g, j, color)) {
        ++i;
      }
    }
  }
  return i;
}
bool HasCycleDfs(const Graph& g, size_t v, std::vector<uint8_t>& color) {
  color[v] = 1;
  bool has_cycle = false;
  if (color[g[v]] == 1) {
    has_cycle = true;
  }
  if (color[g[v]] == 0) {
    if (HasCycleDfs(g, g[v], color)) {
      has_cycle = true;
    }
  }
  color[v] = 2;
  return has_cycle;
}

int main() {
  int n = 0;
  std::cin >> n;
  Graph g(n);
  for (size_t i = 0; i < g.size(); i++) {
    std::cin >> g[i];
    --g[i];
  }
  std::cout << HasCycle(g);
}