#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <set>

struct Graph {
  size_t ver;
  size_t ed;
  std::vector<std::vector<std::pair<int64_t, int64_t>>> edges;
  std::vector<size_t> bridges;
  std::vector<std::pair<int64_t, int64_t>> nums;
  std::vector<uint8_t> color;
  Graph() : ver(0), ed(0) {
  }
};

void Dfsvisit(Graph& g, int64_t v, std::vector<int>& time_in, std::vector<int>& time_up, int time,
              int64_t parent = -1) {
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
        if (time_in[v] < time_up[u.first]) {
          g.bridges.emplace_back(u.second);
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
  for (int64_t i = 0; i < static_cast<int64_t>(g.ver); i++) {
    if (g.color[i] == 0) {
      Dfsvisit(g, i, time_in, time_up, time);
    }
  }
}

void Attack(const Graph& g, std::vector<size_t>& colvo) {
  size_t count = 0;
  for (size_t i = 0; i < g.bridges.size(); i++) {
    if (colvo[g.nums[g.bridges[i]].first] > 2 && colvo[g.nums[g.bridges[i]].second] > 2) {
      ++count;
    }
  }
  size_t nums = 0;
  for (size_t i = 0; i < g.ver; i++) {
    if (g.edges[i].size() < 2) {
      nums += !g.edges[i].empty() ? 1 : 2;
    }
  }
  nums = nums % 2 == 0 ? nums / 2 : nums / 2 + 1;
  if (count > 0) {
    if (nums > 1) {
      std::cout << nums;
      return;
    }
    ++nums;
  }
  std::cout << nums;
}

int main() {
  size_t v = 0;
  size_t e = 0;
  std::cin >> v >> e;
  std::vector<size_t> colvo(v, 0);
  Graph g;
  g.ver = v;
  g.ed = e;
  g.edges = std::vector<std::vector<std::pair<int64_t, int64_t>>>(v);
  g.nums = std::vector<std::pair<int64_t, int64_t>>(e);
  g.color = std::vector<uint8_t>(v, 0);
  for (size_t i = 0; i < e; ++i) {
    int64_t start = 0;
    int64_t end = 0;
    std::cin >> start >> end;
    --start;
    --end;
    g.edges[start].emplace_back(end, i);
    g.edges[end].emplace_back(start, i);
    g.nums[i] = std::pair<int64_t, int64_t>(start, end);
    ++colvo[start];
    ++colvo[end];
  }
  Dfs(g);
  Attack(g, colvo);
}