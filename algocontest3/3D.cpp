// да здравствует dsu
#include <iostream>
#include <vector>
#include <cstdint>

struct Dsug {
  int v;
  std::vector<std::vector<int>> edge;
  std::vector<int> parent;
  std::vector<int> rank;
  int FindSet(const int& ver) {
    if (ver == parent[ver]) {
      return parent[ver];
    }
    return parent[ver] = FindSet(parent[ver]);
  }

  void Union(int& x, int& y) {
    int x_new = FindSet(x);
    int y_new = FindSet(y);
    x = x_new;
    y = y_new;
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

void Kraskal(Dsug& g) {
  int v = 0;
  int e = 0;
  std::cin >> v >> e;
  g.v = v;
  int from = 0;
  int to = 0;
  int weight = 0;
  int bridgeweight = 0;
  g.edge = std::vector<std::vector<int>>();
  g.parent = std::vector<int>(v);
  g.rank = std::vector<int>(v, 0);
  for (int i = 0; i < v; i++) {
    g.parent[i] = i;
  }
  for (int i = 0; i < e; i++) {
    std::cin >> from >> to >> weight;
    --from;
    --to;
    g.edge.emplace_back(std::vector<int>{from, to, weight});
  }
  for (int i = 0; i < static_cast<int>(g.edge.size()); i++) {
    if (g.FindSet(g.edge[i][0]) != g.FindSet(g.edge[i][1])) {
      g.Union(g.edge[i][0], g.edge[i][1]);
      bridgeweight += g.edge[i][2];
    }
  }
  std::cout << bridgeweight;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Dsug g;
  Kraskal(g);
}