#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using Graph = std::vector<std::vector<int>>;

void BFS(const Graph& g, int from, std::vector<bool>& visit) {
  std::queue<int> que;
  que.push(from);
  visit[from] = true;
  while (!que.empty()) {
    int u = que.front();
    que.pop();
    for (int v : g[u]) {
      if (!visit[v]) {
        visit[v] = true;
        que.push(v);
      }
    }
  }
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;
  Graph g(n * m);
  std::vector<std::vector<int>> cart(n);
  std::vector<int> indices(n * m);
  for (int j = 0; j < n; j++) {
    cart[j].reserve(m);
    for (int i = 0; i < m; i++) {
      int ver = 0;
      std::cin >> ver;
      cart[j].emplace_back(ver);
      indices[j * m + i] = j * m + i;
    }
  }
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < m; i++) {
      if (i - 1 >= 0) {
        if (cart[j][i] < cart[j][i - 1]) {
          g[j * m + i].emplace_back(j * m + (i - 1));
        } else if (cart[j][i] > cart[j][i - 1]) {
          g[j * m + (i - 1)].emplace_back(j * m + i);
        } else if (cart[j][i] == cart[j][i - 1]) {
          g[j * m + i].emplace_back(j * m + (i - 1));
          g[j * m + (i - 1)].emplace_back(j * m + i);
        }
      }
      if (j - 1 >= 0) {
        if (cart[j][i] < cart[j - 1][i]) {
          g[j * m + i].emplace_back((j - 1) * m + i);
        } else if (cart[j][i] > cart[j - 1][i]) {
          g[(j - 1) * m + i].emplace_back(j * m + i);
        } else if (cart[j][i] == cart[j - 1][i]) {
          g[j * m + i].emplace_back((j - 1) * m + i);
          g[(j - 1) * m + i].emplace_back(j * m + i);
        }
      }
    }
  }
  std::sort(indices.begin(), indices.end(), [&](int a, int b) {
    int j1 = a / m;
    int i1 = a % m;
    int j2 = b / m;
    int i2 = b % m;
    return cart[j1][i1] < cart[j2][i2];
  });
  std::vector<bool> visit(g.size(), false);
  int drains = 0;
  for (int i : indices) {
    if (!visit[i]) {
      BFS(g, i, visit);
      drains++;
    }
  }
  std::cout << drains << std::endl;
}