#include <iostream>
#include <vector>
#include <cstdint>
#include <set>
#include <algorithm>
#include <queue>

struct Edge {
  size_t end;
  size_t weight;
};

using Graph = std::vector<std::vector<Edge>>;

struct Dop {
  Edge e;
  size_t dist;
  bool operator<(const Dop& other) const {
    return dist > other.dist || (dist == other.dist && e.weight > other.e.weight);
  }
};

size_t Prim(const Graph& g) {
  std::priority_queue<Dop> heap;
  size_t bridges = 0;
  std::vector<bool> used(g.size(), false);
  std::vector<size_t> dist(g.size(), SIZE_MAX);
  heap.push({{0, 0}, 0});
  dist[0] = 0;
  while (!heap.empty()) {
    auto v = heap.top();
    heap.pop();
    if (used[v.e.end]) {
      continue;
    }
    used[v.e.end] = true;
    bridges += v.e.weight;
    for (auto u : g[v.e.end]) {
      if (!used[u.end] && u.weight < dist[u.end]) {
        dist[u.end] = u.weight;
        heap.push({u, dist[u.end]});
      }
    }
  }
  return bridges;
}

int main() {
  size_t v = 0;
  size_t e = 0;
  size_t from = 0;
  size_t to = 0;
  size_t weight = 0;
  std::cin >> v >> e;
  Graph g(v);
  for (size_t i = 0; i < e; i++) {
    std::cin >> from >> to >> weight;
    --from;
    --to;
    Edge end{to, weight};
    Edge start{from, weight};
    g[from].emplace_back(end);
    g[to].emplace_back(start);
  }
  std::cout << Prim(g);
}