#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <set>

struct Graph {
  int ver;
  int ed;
  std::vector<std::vector<std::pair<int, int>>> edges;
  std::vector<int> multi;
  std::set<int> bridges;
  std::vector<uint8_t> color;
  Graph() : ver(0), ed(0) {
  }
};

bool Comparator(const std::pair<std::pair<int, int>, int>& edge1, const std::pair<std::pair<int, int>, int>& edge2) {
  int u1 = edge1.first.first;
  int v1 = edge1.first.second;
  int u2 = edge2.first.first;
  int v2 = edge2.first.second;
  if (u1 > u2) {
    return true;
  }
  if (u1 < u2) {
    return false;
  }
  if (v1 > v2) {
    return true;
  }
  if (v1 < v2) {
    return false;
  }
  return edge1.second < edge2.second;
}

void Dfsvisit(Graph& g, int v, std::vector<int>& time_in, std::vector<int>& time_up, int time, const int& parent = -1) {
  g.color[v] = 1;
  time_in[v] = time_up[v] = ++time;
  for (auto& u : g.edges[v]) {
    if (u.first != parent) {
      if (g.color[u.first] == 1) {
        time_up[v] = std::min(time_up[v], time_in[u.first]);
      }
      if (g.color[u.first] == 0) {
        Dfsvisit(g, u.first, time_in, time_up, time, v);
        time_up[v] = std::min(time_up[v], time_up[u.first]);
        if (time_in[v] < time_up[u.first] && std::find(g.multi.begin(), g.multi.end(), u.second) == g.multi.end()) {
          g.bridges.insert(u.second);
        }
      }
    }
  }
  g.color[v] = 2;
}
void Dfs(Graph& g) {
  std::vector<int> time_in(g.ver);
  std::vector<int> time_up(g.ver);
  int time = 0;
  for (int i = 0; i < g.ver; i++) {
    if (g.color[i] == 0) {
      Dfsvisit(g, i, time_in, time_up, time);
    }
  }
}

void Vvod(Graph& g, int v, int e) {
  g.ver = v;
  g.ed = e;
  int to = 0;
  int from = 0;
  std::vector<std::pair<std::pair<int, int>, int>> checkmulti(g.ed);
  g.edges = std::vector<std::vector<std::pair<int, int>>>(g.ver);
  for (int i = 0; i < g.ed; ++i) {
    std::cin >> from >> to;
    checkmulti[i].first.first = std::min(to, from);
    checkmulti[i].first.second = std::max(from, to);
    checkmulti[i].second = i + 1;
  }
  std::sort(checkmulti.begin(), checkmulti.end(), Comparator);
  int size = g.ed;
  for (int i = 1; i < size; i++) {
    if (checkmulti[i].first.first == checkmulti[i - 1].first.first &&
        checkmulti[i].first.second == checkmulti[i - 1].first.second) {
      g.multi.emplace_back(checkmulti[i].second);
      g.multi.emplace_back(checkmulti[i - 1].second);
      checkmulti.erase(checkmulti.begin() + i);
      size--;
    }
  }
  for (int i = 0; i < g.ed; i++) {
    if (checkmulti[i].first.second != checkmulti[i].first.first) {
      g.edges[checkmulti[i].first.first - 1].emplace_back(checkmulti[i].first.second - 1, checkmulti[i].second);
      g.edges[checkmulti[i].first.second - 1].emplace_back(checkmulti[i].first.first - 1, checkmulti[i].second);
    }
  }
  g.color = std::vector<uint8_t>(v, 0);
  checkmulti.clear();
}
int main() {
  int v = 0;
  int e = 0;
  std::cin >> v >> e;
  Graph g;
  Vvod(g, v, e);
  Dfs(g);
  std::cout << g.bridges.size() << std::endl;
  for (auto elem : g.bridges) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;
}