
#include <iostream>
#include <vector>
#include <cstdint>
#include <set>
#include <algorithm>
#include <queue>

struct Edge {
  int64_t end;
  int64_t weight;
};

using Graph = std::vector<std::vector<Edge>>;

struct Dop {
  int64_t ind;
  int64_t dist;
  Dop(int64_t index, int64_t distance) : ind(index), dist(distance){};
  bool operator<(const Dop& other) const {
    return dist > other.dist || (dist == other.dist && ind > other.ind);
  }
};

void Deikstra(const Graph& g, int64_t start) {
  std::priority_queue<Dop> heap;
  std::vector<bool> used(g.size(), false);
  const int64_t inf = 2009000999;
  std::vector<int64_t> dist(g.size(), inf);
  Dop clast(start, 0);
  heap.emplace(clast);
  dist[start] = 0;
  while (!heap.empty()) {
    auto v = heap.top();
    heap.pop();
    if (used[v.ind]) {
      continue;
    }
    used[v.ind] = true;
    for (auto u : g[v.ind]) {
      if (!used[u.end] && dist[v.ind] + u.weight < dist[u.end]) {
        dist[u.end] = dist[v.ind] + u.weight;
        heap.emplace(u.end, dist[u.end]);
      }
    }
  }
  for (size_t i = 0; i < dist.size(); i++) {
    std::cout << dist[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t k = 0;
  size_t v = 0;
  size_t e = 0;
  std::cin >> k;
  for (size_t j = 0; j < k; j++) {
    std::cin >> v >> e;
    int64_t weight = 0;
    Graph g(v);
    for (size_t i = 0; i < e; i++) {
      int64_t to = 0;
      int64_t from = 0;
      std::cin >> from >> to >> weight;
      Edge start{to, weight};
      g[from].emplace_back(start);
      Edge end{from, weight};
      g[to].emplace_back(end);
    }
    int64_t here = 0;
    std::cin >> here;
    Deikstra(g, here);
  }
}