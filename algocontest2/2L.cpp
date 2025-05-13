#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using Graph = std::vector<std::vector<size_t>>;

void TopSortedDfs(const Graph&, std::vector<uint8_t>&, size_t, std::vector<size_t>&);
void DfsVisit(const Graph&, const size_t&, std::vector<uint8_t>&, std::vector<size_t>&);

std::vector<size_t> TopSort(const Graph& g) {
  std::vector<uint8_t> color(g.size(), 0);
  std::vector<size_t> topsorted;
  topsorted.reserve(g.size());
  for (size_t i = 0; i < g.size(); i++) {
    if (color[i] == 0) {
      TopSortedDfs(g, color, i, topsorted);
    }
  }
  return topsorted;
}

void TopSortedDfs(const Graph& g, std::vector<uint8_t>& color, size_t v, std::vector<size_t>& topsorted) {
  color[v] = 1;
  for (size_t u : g[v]) {
    if (color[u] == 0) {
      TopSortedDfs(g, color, u, topsorted);
    }
  }
  color[v] = 2;
  topsorted.push_back(v);
}

void Transform(Graph& g) {
  Graph vec(g.size());
  for (size_t from = 0; from < g.size(); ++from) {
    for (auto to : g[from]) {
      vec[to].emplace_back(from);
    }
  }
  g = vec;
}

std::vector<std::vector<size_t>> DFS(const Graph& g, std::vector<size_t>& result, std::vector<size_t>& index) {
  std::vector<uint8_t> color(g.size(), 0);
  std::vector<std::vector<size_t>> chats;
  size_t k = 0;
  for (auto elem = result.rbegin(); elem != result.rend(); ++elem) {
    if (color[*elem] == 0) {
      std::vector<size_t> visit;
      DfsVisit(g, *elem, color, visit);
      chats.emplace_back(visit);
      index.emplace_back(++k);
    }
  }
  return chats;
}

void DfsVisit(const Graph& g, const size_t& v, std::vector<uint8_t>& color, std::vector<size_t>& visit) {
  color[v] = 1;
  visit.emplace_back(v);
  for (size_t i = 0; i < g[v].size(); ++i) {
    size_t u = g[v][i];
    if (color[u] == 0) {
      DfsVisit(g, u, color, visit);
    }
  }
  color[v] = 2;
}

/* std::vector<size_t> Des(const std::vector<std::vector<size_t>>& result) {
  std::vector<size_t> index(result.size());
  for (size_t i = 0; i < result.size(); ++i) {
    auto ind = std::min_element(result[i].begin(), result[i].end());
    index[i] = *ind;
  }
  for (size_t i = 1; i < result.size(); i++) {
    for (size_t j = 0; j < result[i].size(); j++) {
      if (result[i][j] < index[i - 1]) {
        std::swap(index[i], index[i - 1]);
      }
    }
  }
  return index;
} */
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  size_t v = 0;
  size_t e = 0;
  std::cin >> v >> e;
  Graph g(v);
  for (size_t i = 0; i < e; ++i) {
    size_t to = 0;
    size_t from = 0;
    std::cin >> from >> to;
    --to;
    --from;
    g[from].emplace_back(to);
  }
  std::vector<size_t> result = TopSort(g);
  Transform(g);
  std::vector<size_t> index;
  std::vector<std::vector<size_t>> resultend = DFS(g, result, index);
  // std::vector<size_t> index = Des(resultend);
  std::cout << resultend.size() << std::endl;
  std::vector<size_t> color(g.size());
  for (size_t j = 0; j < resultend.size(); ++j) {
    for (size_t i : resultend[j]) {
      color[i] = index[j];
    }
  }
  for (size_t i = 0; i < g.size(); ++i) {
    std::cout << color[i] << " ";
  }
  std::cout << std::endl;
  return 0;
}