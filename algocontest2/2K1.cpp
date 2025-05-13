#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

struct Graph {
  int n;
  std::vector<int> color;
  std::vector<std::vector<int>> edges;
  Graph() : n(0) {
  }
};

std::istream& operator>>(std::istream& is, Graph& current) {
  is >> current.n;
  current.edges = std::vector<std::vector<int>>(current.n);
  char color;
  for (int i = 0; i < current.n - 1; i++) {
    for (int j = i + 1; j < current.n; j++) {
      is >> color;
      if (color == 'R') {
        current.edges[i].emplace_back(j);
      } else {
        current.edges[j].emplace_back(i);
      }
    }
  }
  current.color = std::vector<int>(current.n, 0);
  return is;
}

bool CycleCheckVisit(Graph& graph, const int& ver) {
  graph.color[ver] = 1;
  for (auto u : graph.edges[ver]) {
    if (graph.color[u] == 1) {
      return true;
    }
    if (graph.color[u] == 0) {
      if (CycleCheckVisit(graph, u)) {
        return true;
      }
    }
  }
  graph.color[ver] = 2;
  return false;
}

bool CycleCheck(Graph& graph) {
  std::vector<int> route;
  for (int i = 0; i < graph.n; i++) {
    if (graph.color[i] == 0) {
      if (CycleCheckVisit(graph, i)) {
        return true;
      }
    }
  }
  return false;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  if (CycleCheck(graph)) {
    std::cout << "NO\n";
  } else {
    std::cout << "YES\n";
  }
  return 0;
}
