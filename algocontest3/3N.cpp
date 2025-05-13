#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>

struct Edge {
  int64_t c;
  int64_t f;
};

using Graph = std::vector<std::vector<Edge>>;

int64_t DFS(Graph&, int64_t, const int64_t&, std::vector<int64_t>&, int64_t, const int64_t&);

void FordFalkerson(Graph& g, const int64_t& v) {
  int64_t iter = 1;
  std::vector<int64_t> visit(g.size(), 0);
  while (DFS(g, 0, 1000000000, visit, iter, v) != 0) {
    ++iter;
  }
}

int64_t DFS(Graph& g, int64_t v, const int64_t& dflow, std::vector<int64_t>& visit, int64_t iter, const int64_t& gv) {
  if (v == gv - 1) {
    return dflow;
  }
  visit[v] = iter;
  for (int64_t i = 0; i < gv; i++) {
    if (g[v][i].c != g[v][i].f && visit[i] != iter) {
      int64_t flow = DFS(g, i, std::min(dflow, g[v][i].c - g[v][i].f), visit, iter, gv);
      if (flow > 0) {
        g[v][i].f += flow;
        g[i][v].f -= flow;
        return flow;
      }
    }
  }
  return 0;
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
    g[i].resize(v);
    Edge fill{0, 0};
    std::fill(g[i].begin(), g[i].end(), fill);
  }
  for (int64_t i = 0; i < e; i++) {
    int64_t start = 0;
    int64_t end = 0;
    int64_t weight = 0;
    std::cin >> start >> end >> weight;
    --start;
    --end;
    Edge direct{weight, 0};
    g[start][end] = direct;
  }
  FordFalkerson(g, v);
  int64_t flow = 0;
  for (int64_t i = 0; i < v; i++) {
    flow += g[0][i].f;
  }
  std::cout << flow << std::endl;
  return 0;
}
