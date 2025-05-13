
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
  size_t ind;
  size_t dist;
  bool operator<(const Dop& other) const {
    return dist > other.dist || (dist == other.dist && ind > other.ind);
  }
};

size_t Deikstra(const Graph& g, size_t start, size_t end) {
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
  return dist[end];
}

std::ostream& operator<<(std::ostream& os, Edge& e) {
  os << e.end + 1 << " " << e.weight;
  return os;
}

int main() {
  size_t n = 0;
  size_t u = 0;
  size_t d = 0;
  size_t i = 0;
  size_t j = 0;
  size_t l = 0;
  std::cin >> n >> u >> d >> i >> j >> l;
  Graph g(1000000 + l);
  for (size_t k = 0; k < n; k++) {
    if (k == 0) {
      if (k + 1 < n) {
        Edge direct{1, u};
        g[k].emplace_back(direct);
      } else {
        break;
      }
    } else if (k == n - 1 && n - 2 >= 0) {
      Edge undirect{n - 2, d};
      g[k].emplace_back(undirect);
    } else {
      Edge direct{k + 1, u};
      Edge undirect{k - 1, d};
      g[k].emplace_back(direct);
      g[k].emplace_back(undirect);
    }
  }
  size_t colvo = 0;
  size_t max = 0;
  for (size_t r = 0; r < l; r++) {
    std::cin >> colvo;
    for (size_t w = 0; w < colvo; w++) {
      size_t end = 0;
      std::cin >> end;
      --end;
      if (end > max) {
        max = end;
      }
      Edge enter{1000000 + r, i};
      Edge extr{end, j};
      g[end].emplace_back(enter);
      g[1000000 + r].emplace_back(extr);
    }
  }
  if (n - 1 < max) {
    for (size_t k = n - 1; k < max + 1; k++) {
      if (k == n - 1) {
        Edge direct{k + 1, u};
        g[k].emplace_back(direct);
      } else if (k == max) {
        Edge undirect{k - 1, d};
        g[k].emplace_back(undirect);
      } else {
        Edge direct{k + 1, u};
        Edge undirect{k - 1, d};
        g[k].emplace_back(direct);
        g[k].emplace_back(undirect);
      }
    }
  }
  std::cout << Deikstra(g, 0, n - 1) << std::endl;
  return 0;
}