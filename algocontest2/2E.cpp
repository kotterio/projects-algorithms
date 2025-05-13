#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Edges {
  int st;
  int to;
  int weight;
};

using Graph = std::vector<std::vector<std::pair<int, int>>>;

int BFS(const Graph& g, int from, int to) {
  std::vector<int> dist(g.size(), -1);
  std::vector<std::queue<int>> que(30 * (g.size() - 1));
  dist[from] = 0;
  que[0].push(from);
  int i = 0;
  while (!que.empty() && i < static_cast<int>(que.size())) {
    while (!que[i].empty()) {
      int v = que[i].front();
      que[i].pop();
      for (int k = 0; k < static_cast<int>(g[v].size()); k++) {
        int nei = g[v][k].first;
        if (dist[nei] == -1 || dist[nei] > dist[v] + g[v][k].second) {
          dist[nei] = dist[v] + g[v][k].second;
          que[dist[nei]].push(nei);
        }
      }
    }
    i++;
  }
  return dist[to];
}

int main() {
  int v = 0;
  int e = 0;
  std::cin >> v >> e;
  Graph g(v);
  int start = 0;
  int end = 0;
  std::cin >> start >> end;
  --start;
  --end;
  for (int i = 0; i < e; i++) {
    int to = 0;
    int from = 0;
    int weight = 0;
    std::cin >> from >> to >> weight;
    to--;
    from--;
    g[from].emplace_back(to, weight);
  }
  std::cout << BFS(g, start, end);
}
