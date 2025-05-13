#include <iostream>
#include <vector>
#include <climits>

struct Flight {
  int64_t end;
  int64_t cost;
};

int main() {
  int64_t n = 0;
  int64_t m = 0;
  int64_t days = 0;
  int64_t s = 0;
  int64_t f = 0;
  std::cin >> n >> m >> days >> s >> f;
  --s;
  --f;
  if (s == f) {
    std::cout << 0 << std::endl;
    return 0;
  }
  std::vector<std::vector<Flight>> flights(n);
  for (int64_t i = 0; i < m; i++) {
    int64_t start = 0;
    int64_t end = 0;
    int64_t cost = 0;
    std::cin >> start >> end >> cost;
    --start;
    --end;
    flights[start].push_back({end, cost});
  }
  std::vector<std::vector<int64_t>> dp(days + 1, std::vector<int64_t>(n, INT64_MAX));
  dp[0][s] = 0;
  for (int64_t k = 1; k <= days; k++) {
    for (int64_t city = 0; city < n; city++) {
      for (auto flight : flights[city]) {
        if (dp[k - 1][city] != INT64_MAX) {
          dp[k][flight.end] = std::min(dp[k][flight.end], dp[k - 1][city] + flight.cost);
        }
      }
    }
  }
  int64_t res = INT64_MAX;
  for (int64_t u = 0; u <= days; u++) {
    res = std::min(res, dp[u][f]);
  }
  if (res == INT64_MAX) {
    std::cout << -1 << std::endl;
  } else {
    std::cout << res << std::endl;
  }
  return 0;
}
