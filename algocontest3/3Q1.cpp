#include <iostream>
#include <vector>
#include <queue>

struct Edge {
  int64_t c;
  int64_t f;
  bool itdin;
  int64_t index;
  int64_t end;
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
    for (size_t v = 0; v < g[u].size(); v++) {
      if (visited[g[u][v].end] > visited[u] && g[u][v].c - g[u][v].f > 0) {
        if (visited[g[u][v].end] == INT64_MAX) {
          que.push(g[u][v].end);
        }
        visited[g[u][v].end] = visited[u] + 1;
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
  // std::cout << DFS(g, 0, 100000000, visit, v - 1) << std::endl;
  while ((res = DFS(g, 0, 100000000, visit, v - 1)) != 0) {
    flow += res;
  }
  return flow;
}

int64_t DFS(Graph& g, int64_t v, int64_t dflow, std::vector<bool>& visit, int64_t finish) {
  visit[v] = true;
  for (size_t i = 0; i < g[v].size(); i++) {
    if (g[v][i].c != g[v][i].f && g[v][i].itdin && !visit[g[v][i].end]) {
      if (g[v][i].end == finish) {
        int64_t flow = std::min(dflow, g[v][i].c - g[v][i].f);
        int64_t end = g[v][i].end;
        g[v][i].f += flow;
        g[end][g[v][i].index].f -= flow;
        return flow;
      }
      int64_t flow = DFS(g, g[v][i].end, std::min(dflow, g[v][i].c - g[v][i].f), visit, finish);
      if (flow > 0) {
        int64_t end = g[v][i].end;
        g[v][i].f += flow;
        g[end][g[v][i].index].f -= flow;
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
    for (size_t i = 0; i < g.size(); i++) {
      for (auto& elem : g[i]) {
        elem.itdin = false;
      }
    }
  }
  return flow;
}

std::ostream& operator<<(std::ostream& os, Edge& e) {
  os << e.c << " " << e.f << " " << e.itdin << " " << e.index << " " << e.end;
  return os;
}

int main() {
  int64_t v = 0;
  int64_t e = 0;
  std::cin >> v >> e;
  Graph g(v);
  for (int64_t i = 0; i < e; i++) {
    int64_t start = 0;
    int64_t end = 0;
    int64_t weight = 0;
    std::cin >> start >> end >> weight;
    --start;
    --end;
    Edge direct{weight, 0, false, static_cast<int64_t>(g[end].size()), end};
    Edge undirect{0, 0, false, static_cast<int64_t>(g[start].size()), start};
    g[start].emplace_back(direct);
    g[end].emplace_back(undirect);
  }
  std::cout << Din(g, 0, v - 1);
}