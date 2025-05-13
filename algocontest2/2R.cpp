#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdint>

struct Edges {
  size_t st;
  size_t to;
  size_t weight;
};

using Graph = std::vector<std::vector<std::pair<size_t, size_t>>>;

size_t BFS(const Graph& g, size_t from, size_t to, size_t max_weight) {
  std::vector<size_t> dist(g.size(), SIZE_MAX);
  std::vector<std::queue<size_t>> que(max_weight + 1);
  std::vector<bool> used(g.size(), false);
  dist[from] = 0;
  que[0].push(from);
  size_t i = 0;
  while (i < que.size()) {
    bool iter = false;
    for (size_t i = 0; i < que.size(); i++) {
      if (!que[i].empty()) {
        iter = true;
      }
    }
    if (!iter) {
      break;
    }
    while (!que[i].empty()) {
      size_t v = que[i].front();
      que[i].pop();
      if (used[v]) {
        continue;
      }
      if (v == to) {
        return dist[v];
      }
      used[v] = true;
      for (size_t k = 0; k < g[v].size(); k++) {
        size_t nei = g[v][k].first;
        size_t weight = g[v][k].second;
        if (!used[nei] && dist[nei] > dist[v] + weight) {
          dist[nei] = dist[v] + weight;
          que[dist[nei] % (max_weight + 1)].push(nei);
        }
      }
    }
    i = (i + 1) % (max_weight + 1);
  }
  return SIZE_MAX;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t v = 0;
  size_t e = 0;
  std::cin >> v >> e;
  Graph g(v);
  size_t start = 0;
  size_t end = 0;
  std::cin >> start >> end;
  --start;
  --end;
  size_t max_weight = 0;
  for (size_t i = 0; i < e; i++) {
    size_t to = 0;
    size_t from = 0;
    size_t weight = 0;
    std::cin >> from >> to >> weight;
    to--;
    from--;
    g[from].emplace_back(to, weight);
    max_weight = std::max(max_weight, weight);
  }
  size_t res = BFS(g, start, end, max_weight);
  if (res != SIZE_MAX) {
    std::cout << res;
  } else {
    std::cout << -1;
  }
  return 0;
}
