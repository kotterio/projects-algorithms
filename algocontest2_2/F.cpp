#include <vector>
#include <iostream>

class FenwickTree3 {
 public:
  explicit FenwickTree3(std::vector<std::vector<std::vector<int64_t>>>& a) : a_(a) {
    auto n = static_cast<int64_t>(a_.size());
    int64_t m = n;
    int64_t z = m;
    tree_.assign(n, std::vector<std::vector<int64_t>>(m, std::vector<int64_t>(z)));
    for (int64_t i = 0; i < n; ++i) {
      for (int64_t j = 0; j < m; ++j) {
        for (int64_t k = 0; k < z; ++k) {
          Modify(i, j, k, a_[i][j][k]);
        }
      }
    }
  }
  void Set(int64_t i, int64_t j, int64_t k, int64_t x) {
    Modify(i, j, k, x);
    a_[i][j][k] += x;
  }

  int64_t S(int64_t x, int64_t y, int64_t z) {
    int64_t result = 0;
    for (int64_t i = x; i >= 0; i = (i & (i + 1)) - 1) {
      for (int64_t j = y; j >= 0; j = (j & (j + 1)) - 1) {
        for (int64_t k = z; k >= 0; k = (k & (k + 1)) - 1) {
          result += tree_[i][j][k];
        }
      }
    }
    return result;
  }
  int64_t Zfir(int64_t z2, int64_t x1, int64_t x2, int64_t y1, int64_t y2) {
    if (y1 - 1 < 0) {
      if (x1 - 1 < 0) {
        return S(x2, y2, z2);
      }
      return S(x2, y2, z2) - S(x1 - 1, y2, z2);
    }
    if (x1 - 1 < 0) {
      return S(x2, y2, z2) - S(x2, y1 - 1, z2);
    }
    return S(x2, y2, z2) - S(x1 - 1, y2, z2) - S(x2, y1 - 1, z2) + S(x1 - 1, y1 - 1, z2);
  }

  int64_t Xfir(int64_t x2, int64_t y1, int64_t y2, int64_t z1, int64_t z2) {
    if (y1 - 1 < 0) {
      if (z1 - 1 < 0) {
        return S(x2, y2, z2);
      }
      return S(x2, y2, z2) - S(x2, y2, z1 - 1);
    }
    if (z1 - 1 < 0) {
      return S(x2, y2, z2) - S(x2, y1 - 1, z2);
    }
    return S(x2, y2, z2) - S(x2, y1 - 1, z2) - S(x2, y2, z1 - 1) + S(x2, y1 - 1, z1 - 1);
  }

  int64_t Yfir(int64_t y2, int64_t x1, int64_t x2, int64_t z1, int64_t z2) {
    if (x1 - 1 < 0) {
      if (z1 - 1 < 0) {
        return S(x2, y2, z2);
      }
      return S(x2, y2, z2) - S(x2, y2, z1 - 1);
    }
    if (z1 - 1 < 0) {
      // std::cout << S(x2, y2, z2) << " " << S(x1 - 1, y2, z2)<< std::endl;
      return S(x2, y2, z2) - S(x1 - 1, y2, z2);
    }
    return S(x2, y2, z2) - S(x1 - 1, y2, z2) - S(x2, y2, z1 - 1) + S(x1 - 1, y2, z1 - 1);
  }

  int64_t Sum(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t z1, int64_t z2) {
    int64_t result = 0;
    int64_t downkub = 0;
    if (x1 == x2 && y1 == y2 && z1 == z2) {
      return a_[x1][y1][z1];
    }
    if (z1 - 1 < 0) {
      return Zfir(z2, x1, x2, y1, y2);
    }
    if (x1 - 1 < 0) {
      // std::cout << "hi" << std::endl;
      return Xfir(x2, y1, y2, z1, z2);
      // std::cout << "hi" << std::endl;
    }
    if (y1 - 1 < 0) {
      return Yfir(y2, x1, x2, z1, z2);
    }
    result = S(x2, y2, z2) - S(x1 - 1, y2, z2) - S(x2, y1 - 1, z2) + S(x1 - 1, y1 - 1, z2);
    // std::cout << "Sum " << result << std::endl;
    downkub = S(x2, y2, z1 - 1) - S(x1 - 1, y2, z1 - 1) - S(x2, y1 - 1, z1 - 1) + S(x1 - 1, y1 - 1, z1 - 1);
    result -= downkub;
    return result;
  }

 private:
  void Modify(int64_t x, int64_t y, int64_t z, int64_t d) {
    for (int64_t i = x; i < static_cast<int64_t>(tree_.size()); i = i | (i + 1)) {
      for (int64_t j = y; j < static_cast<int64_t>(tree_[0].size()); j = j | (j + 1)) {
        for (int64_t k = z; k < static_cast<int64_t>(tree_.size()); k = k | (k + 1)) {
          tree_[i][j][k] += d;
        }
      }
    }
  }
  std::vector<std::vector<std::vector<int64_t>>> tree_;
  std::vector<std::vector<std::vector<int64_t>>> a_;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int64_t n = 0;
  std::cin >> n;
  std::vector<std::vector<std::vector<int64_t>>> a(
      n + 1, std::vector<std::vector<int64_t>>(n + 1, std::vector<int64_t>(n + 1)));
  FenwickTree3 tree(a);
  int64_t m = 0;
  int64_t x = 0;
  int64_t y = 0;
  int64_t x2 = 0;
  int64_t y2 = 0;
  int64_t z = 0;
  int64_t z2 = 0;
  int64_t k = 0;
  while (true) {
    std::cin >> m;
    if (m == 3) {
      break;
    }
    if (m == 1) {
      std::cin >> x;
      std::cin >> y;
      std::cin >> z;
      std::cin >> k;
      tree.Set(x, y, z, k);
      // std::cout << "change " << tree.S(x, y, z) << std::endl;
    } else {
      std::cin >> x;
      std::cin >> y;
      std::cin >> z;
      std::cin >> x2;
      std::cin >> y2;
      std::cin >> z2;
      std::cout << tree.Sum(x, y, x2, y2, z, z2) << std::endl;
    }
  }
}