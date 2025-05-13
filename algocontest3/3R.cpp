#include <iostream>
#include <vector>
#include <queue>
#include <climits>

struct Edge {
  int64_t from;
  int64_t to;
  int64_t time;
  int64_t arrival;
};

using Graph = std::vector<std::vector<Edge>>;

int main() {
  int64_t n = 0;
  int64_t a = 0;
  int64_t b = 0;
  int64_t k = 0;
  std::cin >> n >> a >> b >> k;
  --a;
  --b;
  Graph graph(n);
  for (int64_t i = 0; i < k; ++i) {
    int64_t from = 0;
    int64_t dep = 0;
    int64_t to = 0;
    int64_t arr = 0;
    std::cin >> from >> dep >> to >> arr;
    --from;
    --to;
    graph[from].push_back({from, to, dep, arr});
  }
  std::priority_queue<std::pair<int64_t, int64_t>> pq;
  std::vector<int64_t> dist(n, INT_MAX);
  dist[a] = 0;
  std::pair<int64_t, int64_t> put{0, a};
  pq.emplace(put);
  while (!pq.empty()) {
    int64_t cur = pq.top().second;
    int64_t time = -pq.top().first;
    pq.pop();
    if (time > dist[cur]) {
      continue;
    }
    for (const auto& edge : graph[cur]) {
      int64_t next_time = std::max(time, edge.time) + (edge.arrival - edge.time);
      if (next_time < dist[edge.to]) {
        dist[edge.to] = next_time;
        std::pair<int64_t, int64_t> puttopiram{-next_time, edge.to};
        pq.push(puttopiram);
      }
    }
  }
  std::cout << (dist[b] == INT_MAX ? -1 : dist[b]) << std::endl;
  return 0;
}
