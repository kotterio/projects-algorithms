//Джонсон

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

struct Edge {
  int u;
  int weight;
  Edge() : u(0), weight(0) {
  }
  Edge(int b, int c) : u(b), weight(c) {
  }
};

struct Graph {
  int n, bridges, max;
  std::vector<std::vector<Edge>> edges;
  std::vector<int> fb_potential;
  std::vector<bool> used;
  std::vector<int> dist;
  Graph() : n(0), bridges(0), max(-100000) {
  }
};

void Relax(Graph& g, int start, Edge& edge) {
  const int inf = 100000;
  std::cout << "here" << g.fb_potential[edge.u] << " " << edge.u << " "<< g.fb_potential[start] <<" " << start << " "<< edge.weight << std::endl;
  if (g.fb_potential[edge.u] > g.fb_potential[start] + edge.weight && g.fb_potential[start] != inf) {
    g.fb_potential[edge.u] = g.fb_potential[start] + edge.weight;
  }
}

void BellmanFord(Graph& graph) {
  for (int i = 0; i < graph.n - 1; i++) {
    for (int j = 0; j < graph.n + 1; j++) {
      for (size_t k = 0; k < graph.edges[j].size(); k++) {
        Relax(graph, j, graph.edges[j][k]);
      }
    }
  }
}

std::istream& operator>>(std::istream& is, Graph& cur) {
  const int inf = 100000;
  is >> cur.n >> cur.bridges;
  int start = 0;
  int end = 0;
  int weight = 0;
  cur.max = -1 * inf;
  cur.edges = std::vector<std::vector<Edge>>(cur.n + 1);
  cur.fb_potential = std::vector<int>(cur.n + 1);
  for (int i = 0; i <= cur.n; i++) {
    cur.fb_potential[i] = inf;
  }
  cur.fb_potential[cur.n] = 0;
  for (int i = 0; i < cur.bridges; i++) {
    is >> start >> end >> weight;
    cur.edges[start].emplace_back(end, weight);
  }
  for (int i = 0; i < cur.n; i++) {
    cur.edges[cur.n].emplace_back(i, 0);
  }
  return is;
}

void Dijkstra(Graph& graph, int start) {
  const int inf = 100000;
  int v_ver = 0;
  graph.used = std::vector<bool>(graph.n, false);
  graph.dist = std::vector<int>(graph.n);
  for (int i = 0; i < graph.n; i++) {
    graph.dist[i] = 100000;
  }
  graph.dist[start] = 0;
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> queue;
  queue.emplace(0, start);
  while (!queue.empty()) {
    std::pair<int, int> cur = queue.top();
    queue.pop();
    v_ver = cur.second;
    if (!graph.used[v_ver]) {
      graph.used[v_ver] = true;
      for (size_t i = 0; i < graph.edges[v_ver].size(); i++) {
        int u_ver = graph.edges[v_ver][i].u;
        if (!graph.used[u_ver] && graph.dist[u_ver] > graph.dist[v_ver] + graph.edges[v_ver][i].weight) {
          graph.dist[u_ver] = graph.dist[v_ver] + graph.edges[v_ver][i].weight;
          queue.emplace(graph.dist[u_ver], u_ver);
        }
      }
    }
  }
  std::cout << "olala" << std::endl;
  for (int i = 0; i < graph.dist.size(); i++) {
    std::cout << graph.dist[i] << " ";
  }
  std::cout << std::endl;
  for (int i = 0; i < graph.n; i++) {
    if (i != start && graph.dist[i] != inf &&
        graph.max < graph.dist[i] - graph.fb_potential[start] + graph.fb_potential[i]) {
      graph.max = graph.dist[i] - graph.fb_potential[start] + graph.fb_potential[i];
    }
  }
}

void Johnson(Graph& g) {
  BellmanFord(g);
  for (int i = 0; i < g.n; i++) {
    for (size_t j = 0; j < g.edges[i].size(); j++) {
      g.edges[i][j].weight += g.fb_potential[i] - g.fb_potential[g.edges[i][j].u];
      std::cout << "tuta" << g.edges[i][j].weight << std::endl;
    }
  }
  for (int i = 0; i < g.n; i++) {
    Dijkstra(g, i);
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  Johnson(graph);
  std::cout << graph.max;
  return 0;
}