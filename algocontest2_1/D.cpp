#include <iostream>
#include <vector>

std::vector<int64_t> P2z(std::vector<int64_t> p, int64_t n) {
  std::vector<int64_t> z(n, 0);
  z[0] = n;
  for (int64_t i = 1; i < n; i++) {
    if (p[i] > 0) {
      z[i - p[i] + 1] = p[i];
    }
  }
  int64_t i = 1;
  while (i < n) {
    int64_t t = i;
    if (z[i] > 0) {
      for (int64_t j = 1; j < z[i]; j++) {
        if (z[i + j] > z[j]) {
          break;
        }
        z[i + j] = std::min(z[j], z[i] - j);
        t = i + j;
      }
    }
    i = t + 1;
  }
  return z;
}

int main() {
  int64_t n = 0;
  std::cin >> n;
  std::vector<int64_t> z(n);
  for (int64_t i = 0; i < n; i++) {
    std::cin >> z[i];
  }
  std::vector<int64_t> result = P2z(z, n);
  for (int64_t j = 0; j < n - 1; j++) {
    std::cout << result[j] << " ";
  }
  std::cout << z[n - 1];
}