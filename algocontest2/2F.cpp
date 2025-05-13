#include <deque>
#include <iostream>
#include <vector>

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using Graph = std::vector<std::vector<std::pair<int, int>>>;

struct Edges {
  int st;
  int to;
  int weight;
};

int BFS(Graph& g, int from, int to) {
  std::vector<int> dist(g.size(), -1);
  std::deque<int> deque;
  deque.push_front(from);
  dist[from] = 0;
  while (!deque.empty()) {
    int v = deque.front();
    deque.pop_front();
    for (int k = 0; k < static_cast<int>(g[v].size()); k++) {
      int nei = g[v][k].first;
      if (dist[nei] == -1 || dist[nei] > dist[v] + g[v][k].second) {
        dist[nei] = dist[v] + g[v][k].second;
        if (g[v][k].second == 0) {
          deque.push_front(nei);
        } else {
          deque.push_back(nei);
        }
      }
    }
  }
  return dist[to];
}

int main() {
  int v = 0;
  int e = 0;
  std::cin >> v >> e;
  Graph g(v);
  for (int i = 0; i < e; i++) {
    int to = 0;
    int from = 0;
    std::cin >> from >> to;
    to--;
    from--;
    g[from].emplace_back(to, 0);
    g[to].emplace_back(from, 1);
  }
  int count = 0;
  std::cin >> count;
  for (int i = 0; i < count; i++) {
    int start = 0;
    int end = 0;
    std::cin >> start >> end;
    --start;
    --end;
    std::cout << BFS(g, start, end) << std::endl;
  }
}