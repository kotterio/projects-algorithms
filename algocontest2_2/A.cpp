#include <iostream>
#include <vector>

int main() {
  size_t n = 0;
  std::cin >> n;
  int64_t nums = 0;
  std::vector<int64_t> prefixsum(n, 0);
  for (size_t i = 0; i < n; i++) {
    std::cin >> nums;
    if (i > 0) {
      prefixsum[i] = nums + prefixsum[i - 1];
      // std::cout << i << " " << prefixsum[i] << std::endl;
    } else {
      prefixsum[i] = nums;
    }
  }
  size_t k = 0;
  std::cin >> k;
  int64_t right = 0;
  int64_t left = 0;
  for (size_t i = 0; i < k - 1; i++) {
    std::cin >> left;
    std::cin >> right;
    if (left - 2 == -1) {
      std::cout << prefixsum[right - 1] << " ";
    } else {
      std::cout << prefixsum[right - 1] - prefixsum[left - 1 - 1] << " ";
    }
  }
  std::cin >> left;
  std::cin >> right;
  if (left - 2 == -1) {
    std::cout << prefixsum[right - 1];
  } else {
    std::cout << prefixsum[right - 1] - prefixsum[left - 1 - 1];
  }
}