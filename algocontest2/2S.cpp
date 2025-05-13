//Двусвязная атака

#include <algorithm>
#include <iostream>
#include <vector>

struct Graph {
  int64_t n, n_edge, time;
  std::vector<std::vector<std::pair<int, int>>> edges;
  std::vector<std::pair<int, int>> edges_save;
  std::vector<int> color;
  std::vector<int> degree;
  std::vector<int> time_in;
  std::vector<int> time_up;
  std::vector<int> bridges;
  Graph() : n(0), n_edge(0), time(0) {
  }
};

std::istream& operator>>(std::istream& is, Graph& current) {
  is >> current.n >> current.n_edge;
  const int64_t inf = 300000;
  int64_t st, end;
  current.bridges = std::vector<int>();
  current.degree = std::vector<int>(current.n, 0);
  current.edges = std::vector<std::vector<std::pair<int, int>>>(current.n);
  current.edges_save = std::vector<std::pair<int, int>>(current.n_edge);
  for (int64_t i = 0; i < current.n_edge; i++) {
    is >> st >> end;
    current.degree[st - 1]++;
    current.degree[end - 1]++;
    current.edges_save[i].first = st - 1;
    current.edges_save[i].second = end - 1;
    current.edges[st - 1].emplace_back(std::pair<int, int>(end - 1, i + 1));
    current.edges[end - 1].emplace_back(std::pair<int, int>(st - 1, i + 1));
  }
  current.time_in = std::vector<int>(current.n, inf);
  current.time_up = std::vector<int>(current.n, inf);
  current.color = std::vector<int>(current.n, 0);
  return is;
}

void DFSVisit(Graph& graph, const int& ver, const int& parent) {
  graph.color[ver] = 1;
  graph.time_in[ver] = graph.time_up[ver] = ++graph.time;
  int64_t num_kids = 0;
  for (auto u : graph.edges[ver]) {
    if (u.first != parent) {
      if (graph.color[u.first] == 1) {
        graph.time_up[ver] = std::min(graph.time_up[ver], graph.time_in[u.first]);
      }
      if (graph.color[u.first] == 0) {
        ++num_kids;
        DFSVisit(graph, u.first, ver);
        graph.time_up[ver] = std::min(graph.time_up[ver], graph.time_up[u.first]);
        if (graph.time_in[ver] < graph.time_up[u.first]) {
          graph.bridges.emplace_back(u.second);
        }
      }
    }
  }
  graph.color[ver] = 2;
}

void DFS(Graph& graph) {
  for (int64_t i = 0; i < graph.n; i++) {
    if (graph.color[i] == 0) {
      DFSVisit(graph, i, -1);
    }
  }
}

int Degree(Graph& graph) {
  int counter = 0;
  for (size_t i = 0; i < graph.edges.size(); i++) {
    if (graph.edges[i].size() < 2) {
      counter += 2 - graph.edges[i].size();
    }
  }
  if (counter % 2 == 0) {
    return counter / 2;
  }
  return counter / 2 + 1;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  if (graph.n == 1) {
    std::cout << 0;
    return 0;
  }
  DFS(graph);
  int counter = Degree(graph), bridges_cnt = 0;
  for (size_t i = 0; i < graph.bridges.size(); i++) {
    if (graph.degree[graph.edges_save[graph.bridges[i] - 1].first] > 2 &&
        graph.degree[graph.edges_save[graph.bridges[i] - 1].second] > 2) {
      bridges_cnt++;
    }
  }
  if (counter > 1 && bridges_cnt > 0) {
    std::cout << counter;
    return 0;
  }
  if (bridges_cnt > 0) {
    counter++;
  }
  std::cout << counter;
  return 0;
}