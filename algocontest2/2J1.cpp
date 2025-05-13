#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

// loading ...

struct Edge {
  int st;
  int end;
  Edge() : st(0), end(0) {
  }
};

struct Graph {
  int n_point, n_edge;
  std::vector<int> color;
  std::vector<int> top_sorted;
  std::vector<std::vector<int>> edges;
  Graph() : n_point(0), n_edge(0) {
  }
};

std::istream& operator>>(std::istream& is, Edge& current) {
  is >> current.st >> current.end;
  return is;
}

std::istream& operator>>(std::istream& is, Graph& current) {
  is >> current.n_point >> current.n_edge;
  current.edges = std::vector<std::vector<int>>(current.n_point);
  Edge a;
  for (int i = 0; i < current.n_edge; i++) {
    is >> a.st >> a.end;
    current.edges[a.st - 1].emplace_back(std::move(a.end - 1));
  }
  current.color = std::vector<int>(current.n_point, 0);
  current.top_sorted = std::vector<int>();
  return is;
}

bool TopSortDef(Graph& graph, const int& ver) {
  graph.color[ver] = 1;
  for (auto u : graph.edges[ver]) {
    if (graph.color[u] == 1) {
      return false;
    }
    if (graph.color[u] == 0) {
      if (!TopSortDef(graph, u)) {
        return false;
      }
    }
  }
  graph.color[ver] = 2;
  graph.top_sorted.emplace_back(ver + 1);
  return true;
}

std::vector<int> TopSort(Graph& graph) {
  for (int i = 0; i < graph.n_point; i++) {
    if (graph.color[i] == 0) {
      if (!TopSortDef(graph, i)) {
        std::vector<int> route;
        return route;
      }
    }
  }
  return graph.top_sorted;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  std::vector<int> sorted = std::move(TopSort(graph));
  if (sorted.empty()) {
    std::cout << -1;
    return 0;
  }
  for (int64_t i = static_cast<int64_t>(sorted.size()) - 1; i >= 0; i--) {
    std::cout << sorted[i] << " ";
  }
  return 0;
}