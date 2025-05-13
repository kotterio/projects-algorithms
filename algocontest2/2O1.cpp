#include <algorithm>
#include <iostream>
#include <vector>

// *screaming internally*

struct Graph {
  int64_t n, n_edge, time;
  std::vector<std::vector<int64_t>> edges;
  std::vector<int64_t> color;
  std::vector<int64_t> time_in;
  std::vector<int64_t> time_up;
  std::vector<int64_t> articulation_points;
  Graph() : n(0), n_edge(0), time(0) {
  }
};

std::istream& operator>>(std::istream& is, Graph& current) {
  is >> current.n >> current.n_edge;
  const int64_t inf = 3000000;
  int one, two, three;
  current.edges = std::vector<std::vector<int64_t>>(current.n + current.n_edge);
  for (int64_t i = 0; i < current.n_edge; i++) {
    is >> one >> two >> three;
    current.edges[one - 1].emplace_back(i + current.n);
    current.edges[two - 1].emplace_back(i + current.n);
    current.edges[three - 1].emplace_back(i + current.n);
    current.edges[i + current.n].emplace_back(one - 1);
    current.edges[i + current.n].emplace_back(two - 1);
    current.edges[i + current.n].emplace_back(three - 1);
  }
  current.articulation_points = std::vector<int64_t>();
  current.time_in = std::vector<int64_t>(current.n_edge + current.n, inf);
  current.time_up = std::vector<int64_t>(current.n_edge + current.n, inf);
  current.color = std::vector<int64_t>(current.n_edge + current.n, 0);
  return is;
}

void DFSVisit(Graph& graph, const int64_t& ver, bool is_root) {
  graph.color[ver] = 1;
  graph.time_in[ver] = graph.time_up[ver] = ++graph.time;
  int64_t num_kids = 0;
  for (size_t i = 0; i < graph.edges[ver].size(); i++) {
    int64_t u = graph.edges[ver][i];
    if (graph.color[u] == 1) {
      graph.time_up[ver] = std::min(graph.time_up[ver], graph.time_in[u]);
    }
    if (graph.color[u] == 0) {
      ++num_kids;
      DFSVisit(graph, u, false);
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
  for (size_t i = graph.n; i < graph.edges.size(); i++) {
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
  std::vector<int64_t> only_pillows;
  for (size_t i = 0; i < graph.articulation_points.size(); i++) {
    if (graph.articulation_points[i] - graph.n > 0) {
      only_pillows.emplace_back(graph.articulation_points[i] - graph.n);
    }
  }
  std::cout << only_pillows.size() << "\n";
  for (size_t i = 0; i < only_pillows.size(); i++) {
    std::cout << only_pillows[i] << "\n";
  }
  return 0;
}