#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

struct Edge {
  int st;
  int end;
  Edge() : st(0), end(0) {
  }
};

struct Graph {
  int n, n_edge;
  std::vector<int> color;
  std::vector<int> top_sorted;
  std::vector<std::vector<int>> edges;
  Graph() : n(0) {
  }
  void Transpose() {
    std::vector<std::vector<int>> vec(n);
    for (int i = 0; i < n; i++) {
      for (size_t j = 0; j < edges[i].size(); j++) {
        vec[edges[i][j]].emplace_back(i);
      }
    }
    edges = vec;
  }
};

std::istream& operator>>(std::istream& is, Edge& current) {
  is >> current.st >> current.end;
  return is;
}

std::istream& operator>>(std::istream& is, Graph& current) {
  is >> current.n >> current.n_edge;
  current.edges = std::vector<std::vector<int>>(current.n);
  Edge a;
  for (int i = 0; i < current.n_edge; i++) {
    is >> a.st >> a.end;
    current.edges[a.st - 1].emplace_back(a.end - 1);
  }
  current.top_sorted = std::vector<int>();
  return is;
}

void TopSortDef(Graph& graph, const int& ver) {
  graph.color[ver] = 1;
  for (auto u : graph.edges[ver]) {
    if (graph.color[u] == 0) {
      TopSortDef(graph, u);
    }
  }
  graph.color[ver] = 2;
  graph.top_sorted.emplace_back(ver + 1);
}

std::vector<int> TopSort(Graph& graph) {
  graph.color = std::vector<int>(graph.n, 0);
  for (int i = 0; i < graph.n; i++) {
    if (graph.color[i] == 0) {
      TopSortDef(graph, i);
    }
  }
  return graph.top_sorted;
}

void DFSVisit(Graph& graph, const int& ver, std::vector<int>& visited, int counter) {
  graph.color[ver] = 1;
  visited[ver] = counter;
  for (auto u : graph.edges[ver]) {
    if (graph.color[u] == 0) {
      DFSVisit(graph, u, visited, counter);
    }
  }
  graph.color[ver] = 2;
}

std::pair<std::vector<int>, int> DFS(Graph& graph, std::vector<int> order) {
  graph.color = std::vector<int>(graph.n, 0);
  int counter = 0;
  std::vector<int> visited(graph.n);
  for (auto elem : order) {
    if (graph.color[elem - 1] == 0) {
      counter++;
      DFSVisit(graph, elem - 1, visited, counter);
    }
  }
  return std::pair<std::vector<int>, int>(visited, counter);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  std::vector<int> order_66 = TopSort(graph);
  std::reverse(order_66.begin(), order_66.end());
  graph.Transpose();
  std::pair<std::vector<int>, int> components = DFS(graph, order_66);
  std::cout << components.second << "\n";
  for (int i = 0; i < graph.n; i++) {
    std::cout << components.first[i] << " ";
  }
  return 0;
}