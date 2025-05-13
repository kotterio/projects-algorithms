
#include <iostream>
#include <vector>
#include <cstdint>
#include <set>
#include <algorithm>
#include <queue>
#include <cstdio>

struct Edge {
  size_t end;
  size_t weight;
};

using Graph = std::vector<std::vector<Edge>>;

struct Dop {
  size_t ind;
  size_t dist;
  bool operator<(const Dop& other) const {
    return dist > other.dist || (dist == other.dist && ind > other.ind);
  }
};

size_t Deikstra(const Graph& g, size_t start, size_t end, std::vector<size_t>& ilnes) {
  std::priority_queue<Dop> heap;
  std::vector<bool> used(g.size(), false);
  std::vector<size_t> dist(g.size(), SIZE_MAX);
  Dop put{start, 0};
  heap.emplace(put);
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
        Dop putnow{u.end, dist[u.end]};
        heap.emplace(putnow);
      }
    }
  }
  for (size_t i = 0; i < ilnes.size(); i++) {
    if (dist[ilnes[i]] <= dist[end]) {
      return SIZE_MAX;
    }
  }
  return dist[end];
}

int main() {
  size_t n = 0;
  size_t m = 0;
  size_t k = 0;
  scanf("%zu %zu %zu", &n, &m, &k);
  std::vector<size_t> ilnes(k);
  for (size_t i = 0; i < k; i++) {
    size_t il = 0;
    scanf("%zu", &il);
    --il;
    ilnes[i] = il;
  }
  size_t weight = 0;
  size_t from = 0;
  size_t to = 0;
  Graph g(n);
  for (size_t i = 0; i < m; i++) {
    scanf("%zu %zu %zu", &from, &to, &weight);
    --from;
    --to;
    Edge start{to, weight};
    Edge end{from, weight};
    g[to].emplace_back(end);
    g[from].emplace_back(start);
  }
  size_t startver = 0;
  size_t endver = 0;
  scanf("%zu %zu", &startver, &endver);
  --startver;
  --endver;
  size_t result = Deikstra(g, endver, startver, ilnes);
  if (result == SIZE_MAX) {
    printf("-1");
    return 0;
  }
  printf("%zu", result);
  return 0;
}