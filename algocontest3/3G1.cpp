//Предатель отрицательного веса

#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

struct Edge {
  int u;
  int v;
  int weight;
  Edge() : u(0), v(0), weight(0) {
  }
  Edge(int a, int b, int c) : u(a), v(b), weight(c) {
  }
};

struct Graph {
  int n, bridges;
  std::vector<Edge> edges;
  std::vector<int> dist;
  Graph() : n(0), bridges(0) {
  }
};

void Relax(Graph& g, Edge& edge) {
  const int inf = 30000;
  if (g.dist[edge.v] > g.dist[edge.u] + edge.weight && g.dist[edge.u] != inf) {
    g.dist[edge.v] = g.dist[edge.u] + edge.weight;
  }
}

void BellmanFord(Graph& graph) {
  for (int i = 0; i < graph.n - 1; i++) {
    for (size_t j = 0; j < graph.edges.size(); j++) {
      Relax(graph, graph.edges[j]);
    }
  }
}

std::istream& operator>>(std::istream& is, Graph& cur) {
  is >> cur.n >> cur.bridges;
  int start = 0;
  int end = 0;
  int weight = 0;
  const int inf = 30000;
  cur.edges = std::vector<Edge>();
  cur.dist = std::vector<int>(cur.n);
  cur.dist[0] = 0;
  for (int i = 1; i < cur.n; i++) {
    cur.dist[i] = inf;
  }
  for (int i = 0; i < cur.bridges; i++) {
    is >> start >> end >> weight;
    cur.edges.emplace_back(start - 1, end - 1, weight);
  }
  return is;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  BellmanFord(graph);
  for (int i = 0; i < graph.n; i++) {
    std::cout << graph.dist[i] << " ";
  }
  return 0;
}
