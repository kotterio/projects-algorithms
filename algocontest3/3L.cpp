
#include <iostream>
#include <vector>
#include <cstdint>
#include <set>
#include <algorithm>
#include <queue>

struct Edge {
  int end;
  int weight;
};

using Graph = std::vector<std::vector<Edge>>;

struct Dop {
  int ind;
  int dist;
  Dop(int index, int distance) : ind(index), dist(distance){};
  bool operator<(const Dop& other) const {
    return dist > other.dist || (dist == other.dist && ind > other.ind);
  }
};

int Deikstra(const Graph& g, std::vector<int>& poten, int start) {
  std::priority_queue<Dop> heap;
  std::vector<bool> used(g.size() - 1, false);
  int inf = 100000;
  std::vector<int> dist(g.size() - 1);
  for (int i = 0; i < static_cast<int>(dist.size()); i++) {
    dist[i] = inf;
  }
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
        Dop polo(u.end, dist[u.end]);
        heap.emplace(polo);
      }
    }
  }
  int maxweight = -1 * inf;
  for (int i = 0; i < static_cast<int>(g.size() - 1); i++) {
    if (i != start && dist[i] != inf && maxweight < dist[i] - poten[start] + poten[i]) {
      maxweight = dist[i] - poten[start] + poten[i];
    }
  }
  return maxweight;
}

void Relax(Edge& edge, int start, std::vector<int>& poten) {
  if (poten[edge.end] > poten[start] + edge.weight && poten[start] != 100000) {
    poten[edge.end] = poten[start] + edge.weight;
  }
}
void Fordbelman(Graph& g, std::vector<int>& poten) {
  for (int i = 0; i < static_cast<int>(g.size() - 2); i++) {
    for (int j = 0; j < static_cast<int>(g.size()); ++j) {
      for (int k = 0; k < static_cast<int>(g[j].size()); ++k) {
        Relax(g[j][k], j, poten);
      }
    }
  }
}

void Johson(Graph& g, std::vector<int>& poten) {
  Fordbelman(g, poten);
  int maxweight = 0;
  for (size_t i = 0; i < g.size() - 1; i++) {
    for (size_t j = 0; j < g[i].size(); j++) {
      g[i][j].weight += poten[i] - poten[g[i][j].end];
    }
  }
  for (int j = 0; j < static_cast<int>(g.size() - 1); j++) {
    int maxweightnow = 0;
    maxweightnow = Deikstra(g, poten, j);
    if (maxweight < maxweightnow) {
      maxweight = maxweightnow;
    }
  }
  std::cout << maxweight;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int v = 0;
  int e = 0;
  std::cin >> v >> e;
  int weight = 0;
  Graph g(v + 1);
  const int inf = 100000;
  std::vector<int> potential(v + 1);
  for (int i = 0; i < v; i++) {
    potential[i] = inf;
  }
  potential[v] = 0;
  for (int i = 0; i < e; i++) {
    int to = 0;
    int from = 0;
    std::cin >> from >> to >> weight;
    Edge start{to, weight};
    g[from].emplace_back(start);
  }
  for (int i = 0; i < v; i++) {
    Edge virt{i, 0};
    g[v].emplace_back(virt);
  }
  // std::cout << 12;
  Johson(g, potential);
}