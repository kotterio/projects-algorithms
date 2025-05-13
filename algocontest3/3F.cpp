// да здравствует dsu

#include <iostream>
#include <vector>
#include <cstdint>
#include <utility>
#include <algorithm>

struct Dsug {
  size_t v;
  std::vector<std::pair<std::pair<size_t, size_t>, size_t>> edge;
  std::vector<size_t> parent;
  std::vector<size_t> rank;
  size_t FindSet(const size_t& ver) {
    if (ver == parent[ver]) {
      return parent[ver];
    }
    return parent[ver] = FindSet(parent[ver]);
  }

  void Union(size_t& x, size_t& y) {
    x = FindSet(x);
    y = FindSet(y);
    if (rank[x] < rank[y]) {
      parent[x] = y;
    } else if (rank[y] < rank[x]) {
      parent[y] = x;
    } else {
      parent[x] = y;
      rank[y] += 1;
    }
  }
};

void Boruvka(Dsug& g) {
  size_t v = 0;
  size_t e = 0;
  std::cin >> v;
  g.v = v;
  size_t bridgeweight = 0;
  g.edge = std::vector<std::pair<std::pair<size_t, size_t>, size_t>>();
  g.parent = std::vector<size_t>(v + 1);
  g.rank = std::vector<size_t>(v + 1, 0);
  for (size_t i = 0; i < v + 1; i++) {
    g.parent[i] = i;
  }
  for (size_t i = 0; i < v; i++) {
    for (size_t j = 0; j < v; j++) {
      size_t val = 0;
      std::cin >> val;
      if (j > i && val != 0) {
        g.edge.emplace_back(std::make_pair(i, j), val);
        ++e;
      }
      // g.weight.emplace_back(weight);
    }
  }
  for (size_t i = 0; i < v; i++) {
    size_t weightmission = 0;
    std::cin >> weightmission;
    g.edge.emplace_back(std::make_pair(i, v), weightmission);
    ++e;
  }
  g.v++;
  v++;
  std::vector<bool> color(e, false);
  size_t dsucount = v;
  while (dsucount > 1) {
    size_t neigh1 = 0;
    size_t neigh2 = 0;
    std::vector<std::vector<int64_t>> min_edge(v);
    for (size_t i = 0; i < v; i++) {
      min_edge[i].resize(2);
      min_edge[i][1] = -1;
    }
    for (size_t i = 0; i < e; i++) {
      // для каждого дерева из T ищем минимальное дерево, связ это дерево с другим
      neigh1 = g.FindSet(g.edge[i].first.first);
      neigh2 = g.FindSet(g.edge[i].first.second);
      if (neigh1 != neigh2) {
        // в разных компонентах связности
        if (min_edge[neigh1][1] == -1 || static_cast<int64_t>(g.edge[i].second) < min_edge[neigh1][1]) {
          min_edge[neigh1][0] = static_cast<int64_t>(i);
          min_edge[neigh1][1] = static_cast<int64_t>(g.edge[i].second);
        }
        if (min_edge[neigh2][1] == -1 || static_cast<int64_t>(g.edge[i].second) < min_edge[neigh2][1]) {
          min_edge[neigh2][0] = static_cast<int64_t>(i);
          min_edge[neigh2][1] = static_cast<int64_t>(g.edge[i].second);
        }
      }
    }
    for (size_t i = 0; i < v; i++) {
      if (min_edge[i][1] != -1) {
        size_t neigh1 = std::min(g.edge[min_edge[i][0]].first.first, g.edge[min_edge[i][0]].first.second);
        size_t neigh2 = std::max(g.edge[min_edge[i][0]].first.first, g.edge[min_edge[i][0]].first.second);
        auto it = std::find_if(g.edge.begin(), g.edge.end(),
                               [neigh1, neigh2](const std::pair<std::pair<size_t, size_t>, size_t>& edge) {
                                 return (edge.first.first == neigh1 && edge.first.second == neigh2);
                               });
        if (it != g.edge.end()) {
          size_t position = std::distance(g.edge.begin(), it);
          if (!color[position]) {
            color[position] = true;
            bridgeweight += min_edge[i][1];
            g.Union(neigh1, neigh2);
            --dsucount;
          }
        }
      }
    }
  }
  std::cout << bridgeweight;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Dsug g;
  Boruvka(g);
}