#include <iostream>
#include <vector>
#include <algorithm>

using Graph = std::vector<std::pair<std::pair<int64_t, int64_t>, int64_t>>;

void Relax(std::pair<std::pair<int64_t, int64_t>, int64_t>& edge, std::vector<int64_t>& dist) {
  if (dist[edge.first.second] > dist[edge.first.first] + edge.second && dist[edge.first.first] != 30000) {
    dist[edge.first.second] = dist[edge.first.first] + edge.second;
  }
}
std::vector<int64_t> Fordbelman(Graph& g, int64_t v_count) {
  int64_t inf = 30000;
  std::vector<int64_t> dist(v_count, inf);
  dist[0] = 0;
  for (int64_t i = 0; i < v_count - 1; i++) {
    for (size_t j = 0; j < g.size(); j++) {
      Relax(g[j], dist);
    }
  }
  return dist;
}

int main() {
  int64_t v = 0;
  int64_t e = 0;
  Graph g;
  std::cin >> v >> e;
  int64_t start = 0;
  int64_t end = 0;
  int64_t weight = 0;
  for (int64_t i = 0; i < e; i++) {
    std::cin >> start >> end >> weight;
    --start;
    --end;
    g.emplace_back(std::make_pair(start, end), weight);
    // g.emplace_back(std::make_pair(end, start), weight);
  }
  std::vector<int64_t> res = Fordbelman(g, v);
  for (size_t i = 0; i < res.size(); i++) {
    std::cout << res[i] << " ";
  }
}