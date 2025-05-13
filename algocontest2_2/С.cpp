#include <vector>
#include <algorithm>
#include <iostream>

class SparseTable {
 public:
  explicit SparseTable(std::vector<int> &arr) {
    n_ = static_cast<int>(arr.size());
    Powers();
    // sum_table.assign(n, std::vector <int>(1));
    min_table_.assign(n_, std::vector<int>(1));
    for (int i = 0; i < n_; ++i) {
      // sum_table[i][0] = arr[i];
      min_table_[i][0] = arr[i];
    }
    for (int i = 0; i < n_; ++i) {
      for (int j = 1; j < static_cast<int>(power2_.size()) && i + power2_[j] - 1 < n_; ++j) {
        // sum_table[i].emplace_back(0);
        min_table_[i].emplace_back(0);
      }
    }
    for (int i = n_ - 1; i >= 0; --i) {
      for (int j = 1; j < static_cast<int>(power2_.size()) && i + power2_[j] - 1 < n_; ++j) {
        // sum_table[i][j] = sum_table[i][j - 1] + sum_table[i + power2[j - 1]][j - 1];
        min_table_[i][j] = std::min(min_table_[i][j - 1], min_table_[i + power2_[j - 1]][j - 1]);
      }
    }
  }
  int MinQuery(int l, int r) {
    return std::min(min_table_[l][power_[r - l + 1]],
                    min_table_[r + 1 - power2_[power_[r - l + 1]]][power_[r - l + 1]]);
  }

 private:
  int n_;
  // std::vector <std::vector <int>> sum_table;
  std::vector<std::vector<int>> min_table_;
  std::vector<int> power_;
  std::vector<int> power2_;
  void Powers() {
    power_.emplace_back(-1);
    power_.emplace_back(0);
    power2_.emplace_back(1);
    for (int number = 1; number <= n_; ++number) {
      if (number == 2 * power2_.back()) {
        power2_.emplace_back(number);
        power_.emplace_back(power_.back() + 1);
      } else {
        power_.emplace_back(power_.back());
      }
    }
  }
};

int main() {
  size_t n = 0;
  size_t q = 0;
  std::cin >> n;
  std::cin >> q;
  std::vector<int> arr(n, 0);
  for (size_t i = 0; i < n; i++) {
    std::cin >> arr[i];
  }
  SparseTable table(arr);
  int l = 0;
  int r = 0;
  for (size_t i = 0; i < q; i++) {
    std::cin >> l;
    l--;
    std::cin >> r;
    r--;
    std::cout << table.MinQuery(l, r) << std::endl;
  }
}
