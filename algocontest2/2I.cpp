#include <iostream>
#include <vector>

#include <string>
#include <cstdint>
#include <algorithm>

using Graph = std::vector<std::vector<size_t>>;
void DfsVisit(const Graph&, const size_t&, std::vector<uint8_t>&, std::vector<size_t>&);

std::vector<std::vector<size_t>> DFS(const Graph& g) {
  std::vector<uint8_t> color(g.size(), 0);
  std::vector<std::vector<size_t>> chats;
  for (size_t j = 0; j < g.size(); j++) {
    if (color[j] == 0) {
      std::vector<size_t> visit;
      DfsVisit(g, j, color, visit);
      chats.emplace_back(visit);
    }
  }
  return chats;
}

void DfsVisit(const Graph& g, const size_t& v, std::vector<uint8_t>& color, std::vector<size_t>& visit) {
  color[v] = 1;
  visit.emplace_back(v + 1);
  for (size_t i = 0; i < g[v].size(); ++i) {
    size_t u = g[v][i];
    if (color[u] == 0) {
      DfsVisit(g, u, color, visit);
    }
  }
  color[v] = 2;
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
      g[to].emplace_back(from);
    }
  }
  auto chats = DFS(g);
  std::cout << chats.size() << std::endl;
  for (size_t i = 0; i < chats.size(); i++) {
    std::cout << chats[i].size() << std::endl;
    for (size_t j = 0; j < chats[i].size() - 1; j++) {
      std::cout << chats[i][j] << " ";
    }
    std::cout << chats[i][chats[i].size() - 1] << std::endl;
  }
}