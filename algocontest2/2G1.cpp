#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

// still to check

struct Edge {
  int st;
  int end;
  Edge() : st(0), end(0) {
  }
};

struct Graph {
  int n_point, n_edge;
  std::vector<std::vector<int>> edges;
  std::vector<int> color;
  std::vector<int> parent;
  Graph() : n_point(0), n_edge(0) {
  }
};

std::istream& operator>>(std::istream& is, Edge& current) {
  is >> current.st >> current.end;
  return is;
}

std::istream& operator>>(std::istream& is, Graph& current) {
  is >> current.n_point >> current.n_edge;
  std::vector<std::vector<int>> vec(current.n_point);
  Edge a;
  const int inf = 100000;
  for (int i = 0; i < current.n_edge; i++) {
    is >> a.st >> a.end;
    vec[a.st - 1].emplace_back(std::move(a.end - 1));
  }
  current.color = std::vector<int>(current.n_point, 0);
  current.edges = std::move(vec);
  current.parent = std::vector<int>(current.n_point, inf);
  return is;
}

std::pair<bool, int> DFSVisit(Graph& graph, const int& ver) {
  graph.color[ver] = 1;
  for (auto u : graph.edges[ver]) {
    if (graph.color[u] == 1) {
      graph.parent[u] = ver;
      return std::pair<bool, int>(true, u);
    }
    if (graph.color[u] == 0) {
      graph.parent[u] = ver;
      std::pair<bool, int> current = DFSVisit(graph, u);
      if (current.first) {
        return std::pair<bool, int>(true, current.second);
      }
    }
  }
  graph.color[ver] = 2;
  return std::pair<bool, int>(false, -1);
}

std::pair<bool, std::vector<int>> DFS(Graph& graph) {
  std::vector<int> route;
  int finish;
  for (int i = 0; i < graph.n_point; i++) {
    if (graph.color[i] == 0) {
      std::pair<bool, int> current = DFSVisit(graph, i);
      if (current.first) {
        finish = graph.parent[current.second];
        while (finish != current.second) {
          route.emplace_back(std::move(finish + 1));
          finish = graph.parent[finish];
        }
        route.emplace_back(std::move(finish + 1));
        return std::pair<bool, std::vector<int>>(true, route);
      }
    }
  }
  return std::pair<bool, std::vector<int>>(false, route);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  std::pair<bool, std::vector<int>> cycles = DFS(graph);
  if (cycles.first) {
    std::cout << "YES\n";
    for (int64_t i = static_cast<int64_t>(cycles.second.size()) - 1; i >= 0; i--) {
      std::cout << cycles.second[i] << " ";
    }
    std::cout << "\n";
  } else {
    std::cout << "NO\n";
  }
  return 0;
}
