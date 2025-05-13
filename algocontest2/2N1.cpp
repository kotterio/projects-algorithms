#include <algorithm>
#include <iostream>
#include <vector>

bool Comp(const std::pair<std::pair<int, int>, int>& first, const std::pair<std::pair<int, int>, int>& second) {
  return first.first.first >
         second.first.first + (first.first.first == second.first.first
                                   ? second.first.second - first.first.second +
                                         (first.first.second == second.first.second ? -second.second + first.second : 0)
                                   : 0);
}

struct Graph {
  int32_t n, n_edge, time;
  std::vector<std::vector<std::pair<int, int>>> edges;
  std::vector<int> color;
  std::vector<int> exceptions;
  std::vector<int> time_in;
  std::vector<int> time_up;
  std::vector<int> bridges;
  Graph() : n(0), n_edge(0), time(0) {
  }
};

std::istream& operator>>(std::istream& is, Graph& current) {
  is >> current.n >> current.n_edge;
  const int32_t inf = 300000;
  int32_t st, end, size;
  std::vector<std::pair<std::pair<int, int>, int>> check(current.n_edge);
  current.bridges = std::move(std::vector<int>());
  current.exceptions = std::move(std::vector<int>());
  current.edges = std::vector<std::vector<std::pair<int, int>>>(current.n);
  for (int32_t i = 0; i < current.n_edge; i++) {
    is >> st >> end;
    check[i].first.first = std::move(std::min(st, end));
    check[i].first.second = std::move(std::max(st, end));
    check[i].second = i + 1;
  }
  std::sort(check.begin(), check.end(), Comp);
  size = current.n_edge;
  for (int32_t i = 1; i < size; i++) {
    if (check[i].first.first == check[i - 1].first.first && check[i].first.second == check[i - 1].first.second) {
      current.exceptions.emplace_back(check[i].second);
      current.exceptions.emplace_back(check[i - 1].second);
      check.erase(check.begin() + i);
      size--;
    }
  }
  for (size_t i = 0; i < check.size(); i++) {
    if (check[i].first.second != check[i].first.first) {
      current.edges[check[i].first.first - 1].emplace_back(
          std::move(std::pair<int, int>(check[i].first.second - 1, check[i].second)));
      current.edges[check[i].first.second - 1].emplace_back(
          std::move(std::pair<int, int>(check[i].first.first - 1, check[i].second)));
    }
  }
  check.clear();
  current.time_in = std::move(std::vector<int>(current.n, inf));
  current.time_up = std::move(std::vector<int>(current.n, inf));
  current.color = std::move(std::vector<int>(current.n, 0));
  return is;
}

void DFSVisit(Graph& graph, const int& ver, const int& parent = -1) {
  graph.color[ver] = 1;
  graph.time_in[ver] = graph.time_up[ver] = std::move(++graph.time);
  int32_t num_kids = 0;
  for (auto u : graph.edges[ver]) {
    if (u.first != parent) {
      if (graph.color[u.first] == 1) {
        graph.time_up[ver] = std::move(std::min(graph.time_up[ver], graph.time_in[u.first]));
      }
      if (graph.color[u.first] == 0) {
        ++num_kids;
        DFSVisit(graph, u.first, ver);
        graph.time_up[ver] = std::move(std::min(graph.time_up[ver], graph.time_up[u.first]));
        if (graph.time_in[ver] < graph.time_up[u.first]) {
          if (std::find(graph.exceptions.begin(), graph.exceptions.end(), u.second) == graph.exceptions.end()) {
            graph.bridges.emplace_back(u.second);
          }
        }
      }
    }
  }
  graph.color[ver] = 2;
}

void DFS(Graph& graph) {
  for (int32_t i = 0; i < graph.n; i++) {
    if (graph.color[i] == 0) {
      DFSVisit(graph, i);
    }
  }
}

int32_t main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::cin >> graph;
  DFS(graph);
  std::sort(graph.bridges.begin(), graph.bridges.end());
  graph.bridges.erase(std::unique(graph.bridges.begin(), graph.bridges.end()), graph.bridges.end());
  std::cout << graph.bridges.size() << "\n";
  for (size_t i = 0; i < graph.bridges.size(); i++) {
    std::cout << graph.bridges[i] << "\n";
  }
  return 0;
}
