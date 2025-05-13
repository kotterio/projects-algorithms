
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

struct Edge {
  int64_t flow;
  int64_t cap;
  Edge() : flow(0), cap(0) {
  }
  Edge(int64_t cur_flow, int64_t capacity) : flow(cur_flow), cap(capacity) {
  }
};

struct Graph {
  int64_t n;
  int64_t iter;
  int64_t bridges;
  std::vector<std::vector<Edge>> edges;
  std::vector<int64_t> used;
  Graph() : n(0), iter(1), bridges(0) {
  }
};

std::istream& operator>>(std::istream& is, Graph& cur) {
  int64_t start = 0;
  int64_t end = 0;
  int64_t capacity = 0;
  is >> cur.n >> cur.bridges;
  cur.edges = std::vector<std::vector<Edge>>(cur.n);
  cur.used = std::vector<int64_t>(cur.n, 0);
  for (int64_t i = 0; i < cur.n; i++) {
    cur.edges[i].resize(cur.n);
    std::fill(cur.edges[i].begin(), cur.edges[i].end(), Edge(0, 0));
  }
  for (int64_t i = 0; i < cur.bridges; i++) {
    is >> start >> end >> capacity;
    cur.edges[start - 1][end - 1] = Edge(0, capacity);
    // cur.edges[end - 1][start - 1] = Edge(0, 0);
  }
  return is;
}

int64_t DFS(Graph& graph, int64_t v, const int64_t& delta_flow) {
  if (v == graph.n - 1) {
    return delta_flow;
  }
  graph.used[v] = graph.iter;
  for (int64_t i = 0; i < graph.n; i++) {
    if (graph.edges[v][i].cap != graph.edges[v][i].flow && graph.used[i] != graph.iter) {
      int64_t new_flow = DFS(graph, i, std::min(delta_flow, graph.edges[v][i].cap - graph.edges[v][i].flow));
      if (new_flow > 0) {
        graph.edges[v][i].flow += new_flow;
        graph.edges[i][v].flow -= new_flow;
        return new_flow;
      }
    }
  }
  return 0;
}

int64_t FordFalkerson(Graph& graph) {
  int64_t total_flow = 0;
  while (DFS(graph, 0, 1000000000)) {
    ++graph.iter;
  }
  for (int64_t i = 0; i < graph.n; i++) {
    total_flow += graph.edges[0][i].flow;
  }
  return total_flow;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  std::cout << FordFalkerson(graph);
  return 0;
}