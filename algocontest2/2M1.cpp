

#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

// please without reject i'm dead inside

struct Edge {
  int64_t st;
  int64_t end;
  Edge() : st(0), end(0) {
  }
};

struct Graph {
  int64_t n, n_edge, time;
  std::vector<std::vector<int64_t>> edges;
  std::vector<int64_t> color;
  std::vector<int64_t> time_in;
  std::vector<int64_t> time_up;
  std::vector<int64_t> articulation_points;
  Graph() : n(0), n_edge(0), time(0) {
  }
  void Transpose() {
    std::vector<std::vector<int64_t>> vec(n);
    for (int64_t i = 0; i < n; i++) {
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
  const int64_t inf = 1000000;
  Edge a;
  current.edges = std::vector<std::vector<int64_t>>(current.n);
  for (int64_t i = 0; i < current.n_edge; i++) {
    is >> a.st >> a.end;
    current.edges[a.st - 1].emplace_back(a.end - 1);
    current.edges[a.end - 1].emplace_back(a.st - 1);
  }
  current.articulation_points = std::vector<int64_t>();
  current.time_in = std::vector<int64_t>(current.n, inf);
  current.time_up = std::vector<int64_t>(current.n, inf);
  current.color = std::vector<int64_t>(current.n, 0);
  return is;
}

void DFSVisit(Graph& graph, const int64_t& ver, bool is_root) {
  graph.color[ver] = 1;
  graph.time_in[ver] = graph.time_up[ver] = ++graph.time;
  int64_t num_kids = 0;
  for (auto u : graph.edges[ver]) {
    if (graph.color[u] == 1) {
      graph.time_up[ver] = std::min(graph.time_up[ver], graph.time_in[u]);
    }
    if (graph.color[u] == 0) {
      ++num_kids;
      DFSVisit(graph, u, false);
      // std::cout << num_kids << "\n";
      graph.time_up[ver] = std::min(graph.time_up[ver], graph.time_up[u]);
      if (!is_root && graph.time_in[ver] <= graph.time_up[u]) {
        graph.articulation_points.emplace_back(ver + 1);
      }
    }
  }
  if (is_root && num_kids > 1) {
    graph.articulation_points.emplace_back(ver + 1);
  }
  graph.color[ver] = 2;
}

void DFS(Graph& graph) {
  std::vector<int64_t> route;
  for (int64_t i = 0; i < graph.n; i++) {
    if (graph.color[i] == 0) {
      DFSVisit(graph, i, true);
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  DFS(graph);
  std::sort(graph.articulation_points.begin(), graph.articulation_points.end());
  graph.articulation_points.erase(std::unique(graph.articulation_points.begin(), graph.articulation_points.end()),
                                  graph.articulation_points.end());
  std::cout << graph.articulation_points.size() << "\n";
  for (size_t i = 0; i < graph.articulation_points.size(); i++) {
    std::cout << graph.articulation_points[i] << "\n";
  }
  return 0;
}