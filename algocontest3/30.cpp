#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <queue>

using Graph = std::vector<std::vector<int64_t>>;

bool BFS(Graph&, std::vector<int64_t>&, std::vector<bool>&);

int64_t EdmondKarp(Graph& g, int64_t v) {
  int64_t flow = 0;
  int64_t son = 0;
  int64_t dad = 0;
  std::vector<int64_t> parent(g.size(), -1);
  std::vector<bool> visit(g.size(), false);
  while (BFS(g, parent, visit)) {
    int64_t dflow = INT16_MAX;
    son = v - 1;
    while (son != 0) {
      dad = parent[son];
      dflow = std::min(dflow, g[dad][son]);
      son = dad;
    }
    son = v - 1;
    while (son != 0) {
      dad = parent[son];
      g[dad][son] -= dflow;
      g[son][dad] += dflow;
      son = dad;
    }
    flow += dflow;
  }
  return flow;
}

bool BFS(Graph& g, std::vector<int64_t>& parent, std::vector<bool>& visit) {
  for (size_t i = 0; i < parent.size(); i++) {
    parent[i] = -1;
  }
  std::fill(visit.begin(), visit.end(), false);
  std::queue<int64_t> que;
  parent[0] = 0;
  que.push(0);
  while (!que.empty()) {
    int64_t v = que.front();
    que.pop();
    for (int64_t i = 0; i < static_cast<int64_t>(g.size()); i++) {
      if (parent[i] == -1 && !visit[i] && g[v][i] > 0) {
        visit[i] = true;
        parent[i] = v;
        que.push(i);
      }
    }
  }
  return (parent[g.size() - 1] != -1);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int64_t v = 0;
  int64_t e = 0;
  std::cin >> v >> e;
  Graph g(v);
  for (int64_t i = 0; i < v; i++) {
    g[i].reserve(v);
    std::fill(g[i].begin(), g[i].end(), 0);
  }
  for (int64_t i = 0; i < e; i++) {
    int64_t start = 0;
    int64_t end = 0;
    int64_t weight = 0;
    std::cin >> start >> end >> weight;
    --start;
    --end;
    g[start][end] = weight;
  }
  std::cout << EdmondKarp(g, v);
}