#include <iostream>
#include <vector>
#include <queue>

struct Edge {
  int64_t c;
  int64_t f;
  bool itdin;
};

using Graph = std::vector<std::vector<Edge>>;
int64_t DFS(Graph& g, int64_t v, int64_t dflow, std::vector<bool>& visit, int64_t finish);

bool BFS(Graph& g, int64_t start, int64_t finish) {
  std::queue<int64_t> que;
  std::vector<int64_t> visited(g.size(), INT64_MAX);
  que.push(start);
  visited[start] = 0;
  while (!que.empty()) {
    int64_t u = que.front();
    que.pop();
    for (int64_t v = 0; v < static_cast<int64_t>(g[u].size()); v++) {
      if (visited[v] > visited[u] && g[u][v].c - g[u][v].f > 0) {
        if (visited[v] == INT64_MAX) {
          que.push(v);
        }
        visited[v] = visited[u] + 1;
        g[u][v].itdin = true;
      }
    }
  }
  return visited[finish] != INT64_MAX;
}

int64_t FordFalkerson(Graph& g, int64_t& v) {
  std::vector<bool> visit(g.size(), false);
  int64_t flow = 0;
  int64_t res = 0;
  while ((res = DFS(g, 0, 100000000, visit, v - 1)) != 0) {
    flow += res;
  }
  return flow;
}

int64_t DFS(Graph& g, int64_t v, int64_t dflow, std::vector<bool>& visit, int64_t finish) {
  visit[v] = true;
  for (int64_t i = 0; i < static_cast<int64_t>(g[v].size()); i++) {
    if (g[v][i].c != g[v][i].f && g[v][i].itdin && !visit[i]) {
      if (i == finish) {
        int64_t flow = std::min(dflow, g[v][i].c - g[v][i].f);
        g[v][i].f += flow;
        g[i][v].f -= flow;
        return flow;
      }
      int64_t flow = DFS(g, i, std::min(dflow, g[v][i].c - g[v][i].f), visit, finish);
      if (flow > 0) {
        g[v][i].f += flow;
        g[i][v].f -= flow;
        return flow;
      }
    }
  }
  return 0;
}

int64_t Din(Graph& g, int64_t start, int64_t finish) {
  int64_t v = finish + 1;
  int64_t flow = 0;
  while (BFS(g, start, finish)) {
    flow += FordFalkerson(g, v);
    for (int64_t i = 0; i < static_cast<int64_t>(g.size()); i++) {
      for (auto& elem : g[i]) {
        elem.itdin = false;
      }
    }
  }
  return flow;
}

std::ostream& operator<<(std::ostream& os, Edge& e) {
  os << e.c << " " << e.f << " " << e.itdin;
  return os;
}

int main() {
  int64_t v = 0;
  int64_t e = 0;
  std::cin >> v >> e;
  Graph g(v);
  for (int64_t i = 0; i < v; i++) {
    g[i].resize(v);
    Edge fill{0, 0, false};
    std::fill(g[i].begin(), g[i].end(), fill);
  }
  for (int64_t i = 0; i < e; i++) {
    int64_t start = 0;
    int64_t end = 0;
    int64_t weight = 0;
    std::cin >> start >> end >> weight;
    --start;
    --end;
    Edge direct{weight, 0, false};
    Edge undirect{0, 0, false};
    g[start][end] = direct;
    g[end][start] = undirect;
  }
  std::cout << Din(g, 0, v - 1);
}