#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using Graph = std::vector<std::vector<std::pair<size_t, char>>>;
bool Cyclevisitwithcolor(const Graph&, std::vector<uint8_t>&, size_t v);
void DFSwithcolor(Graph& g) {
  std::vector<uint8_t> visit(g.size());
  for (size_t i = 0; i < g.size() - 1; i++) {
    for (size_t j = i + 1; j < g.size(); j++) {
      char color = 0;
      std::cin >> color;
      if (color == 'R') {
        g[i].emplace_back(j, 'R');
      } else {
        g[j].emplace_back(i, 'B');
      }
    }
  }
  for (size_t k = 0; k < g.size(); k++) {
    if (visit[k] == 0) {
      if (Cyclevisitwithcolor(g, visit, k)) {
        std::cout << "NO" << std::endl;
        return;
      }
    }
  }
  // cиние к i
  std::cout << "YES" << std::endl;
}

bool Cyclevisitwithcolor(const Graph& g, std::vector<uint8_t>& visit, size_t v) {
  visit[v] = 1;
  for (auto neighbor : g[v]) {
    if (visit[neighbor.first] == 1) {
      return true;
    }
    if (visit[neighbor.first] == 0) {
      if (Cyclevisitwithcolor(g, visit, neighbor.first)) {
        return true;
      }
    }
  }
  visit[v] = 2;
  return false;
}

int main() {
  size_t v = 0;
  std::cin >> v;
  Graph g(v);
  DFSwithcolor(g);
}