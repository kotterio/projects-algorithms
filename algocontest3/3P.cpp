#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <string>
#include <queue>

struct Edge {
  int64_t c;
  int64_t f;
  bool itdin;
  int64_t index;
  int64_t end;
};

struct Position {
  int64_t a;
  int64_t b;
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

int64_t FordFalkerson(Graph& g, int64_t& start, int64_t finsh) {
  std::vector<bool> visit(g.size(), false);
  int64_t flow = 0;
  int64_t res = 0;
  // std::cout << DFS(g, 0, 100000000, visit, v - 1) << std::endl;
  while ((res = DFS(g, start, 100000000, visit, finsh)) != 0) {
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
  int64_t flow = 0;
  while (BFS(g, start, finish)) {
    flow += FordFalkerson(g, start, finish);
    for (size_t i = 0; i < g.size(); i++) {
      for (auto& elem : g[i]) {
        elem.itdin = false;
      }
    }
  }
  return flow;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int64_t n = 0;
  int64_t m = 0;
  std::cin >> n >> m;
  char vershina = 'P';
  std::string ver = "P";
  std::vector<int64_t> pos(2);
  std::vector<std::pair<char, Position>> indexchet;
  std::vector<std::pair<char, Position>> indexnechet;
  for (int64_t i = 0; i < n; i++) {
    std::cin >> ver;
    for (int64_t j = 0; j < m; j++) {
      vershina = ver[j];
      if (vershina != '.') {
        if ((i + j) % 2 == 0) {
          Position pos{i, j};
          indexchet.emplace_back(vershina, pos);
        } else {
          Position pos{i, j};
          indexnechet.emplace_back(vershina, pos);
        }
      }
    }
  }
  int64_t v = n * m;
  Graph g(v + 2);
  int64_t chetsum = 0;
  for (auto elem : indexchet) {
    if (elem.second.a > 0) {
      Edge direct{1, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b - m].size()),
                  elem.second.a * m + elem.second.b - m};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                    elem.second.a * m + elem.second.b};
      g[elem.second.a * m + elem.second.b].emplace_back(direct);
      g[elem.second.a * m + elem.second.b - m].emplace_back(undirect);
    }
    if (elem.second.a < n - 1) {
      Edge direct{1, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b + m].size()),
                  elem.second.a * m + elem.second.b + m};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                    elem.second.a * m + elem.second.b};
      g[elem.second.a * m + elem.second.b].emplace_back(direct);
      g[elem.second.a * m + elem.second.b + m].emplace_back(undirect);
    }
    if (elem.second.b > 0) {
      Edge direct{1, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b - 1].size()),
                  elem.second.a * m + elem.second.b - 1};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                    elem.second.a * m + elem.second.b};
      g[elem.second.a * m + elem.second.b].emplace_back(direct);
      g[elem.second.a * m + elem.second.b - 1].emplace_back(undirect);
    }
    if (elem.second.b < m - 1) {
      Edge direct{1, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b + 1].size()),
                  elem.second.a * m + elem.second.b + 1};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                    elem.second.a * m + elem.second.b};
      g[elem.second.a * m + elem.second.b].emplace_back(direct);
      g[elem.second.a * m + elem.second.b + 1].emplace_back(undirect);
    }
    if (elem.first == 'H') {
      Edge direct{1, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                  elem.second.a * m + elem.second.b};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[v].size()), v};
      g[v].emplace_back(direct);
      g[elem.second.a * m + elem.second.b].emplace_back(undirect);
      chetsum += 1;
    } else if (elem.first == 'O') {
      Edge direct{2, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                  elem.second.a * m + elem.second.b};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[v].size()), v};
      g[v].emplace_back(direct);
      g[elem.second.a * m + elem.second.b].emplace_back(undirect);
      chetsum += 2;
    } else if (elem.first == 'N') {
      Edge direct{3, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                  elem.second.a * m + elem.second.b};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[v].size()), v};
      g[v].emplace_back(direct);
      g[elem.second.a * m + elem.second.b].emplace_back(undirect);
      chetsum += 3;
    } else if (elem.first == 'C') {
      Edge direct{4, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                  elem.second.a * m + elem.second.b};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[v].size()), v};
      g[v].emplace_back(direct);
      g[elem.second.a * m + elem.second.b].emplace_back(undirect);
      chetsum += 4;
    }
  }
  int64_t nochetsum = 0;
  for (auto elem : indexnechet) {
    if (elem.first == 'H') {
      Edge direct{1, 0, false, static_cast<int64_t>(g[v + 1].size()), v + 1};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                    elem.second.a * m + elem.second.b};
      g[elem.second.a * m + elem.second.b].emplace_back(direct);
      g[v + 1].emplace_back(undirect);
      nochetsum += 1;
    } else if (elem.first == 'O') {
      Edge direct{2, 0, false, static_cast<int64_t>(g[v + 1].size()), v + 1};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                    elem.second.a * m + elem.second.b};
      g[elem.second.a * m + elem.second.b].emplace_back(direct);
      g[v + 1].emplace_back(undirect);
      nochetsum += 2;
    } else if (elem.first == 'N') {
      Edge direct{3, 0, false, static_cast<int64_t>(g[v + 1].size()), v + 1};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                    elem.second.a * m + elem.second.b};
      g[elem.second.a * m + elem.second.b].emplace_back(direct);
      g[v + 1].emplace_back(undirect);
      nochetsum += 3;
    } else if (elem.first == 'C') {
      Edge direct{4, 0, false, static_cast<int64_t>(g[v + 1].size()), v + 1};
      Edge undirect{0, 0, false, static_cast<int64_t>(g[elem.second.a * m + elem.second.b].size()),
                    elem.second.a * m + elem.second.b};
      g[elem.second.a * m + elem.second.b].emplace_back(direct);
      g[v + 1].emplace_back(undirect);
      nochetsum += 4;
    }
  }
  int64_t flow = Din(g, v, v + 1);
  if (chetsum == 0 && nochetsum == 0) {
    std::cout << "Invalid";
  } else if (flow == chetsum && chetsum == nochetsum) {
    std::cout << "Valid";
  } else {
    std::cout << "Invalid";
  }
  return 0;
}