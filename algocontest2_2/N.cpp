#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

const int64_t kNeutralElement = INT64_MAX;
struct SegmentTree {
  std::vector<std::vector<int64_t>> tree;
  std::vector<std::vector<int64_t>> a;
  int64_t size_n = 0;
  int64_t size_m = 0;
  int64_t n = 0;
  int64_t m = 0;
  int64_t Nextpoweroftwo(int64_t n) {
    if (n == 0) {
      return 1;
    }
    return static_cast<int64_t>(std::pow(2, std::ceil(std::log2(n))));
  }
  SegmentTree() = default;
  // n - за строки
  // m - за столбцы
  explicit SegmentTree(const std::vector<std::vector<int64_t>>& b, int64_t n_size, int64_t m_size)
      : a(b), n(n_size), m(m_size) {
    size_m = Nextpoweroftwo(m);
    size_n = Nextpoweroftwo(n);
    for (size_t i = 0; i < a.size(); i++) {
      a[i].resize(size_m, kNeutralElement);
    }
    a.resize(size_n, std::vector<int64_t>(size_m, kNeutralElement));
    tree = std::vector<std::vector<int64_t>>(2 * size_n - 1);
    for (int64_t i = 0; i < 2 * size_n - 1; i++) {
      tree[i].resize(2 * size_m - 1, kNeutralElement);
    }
    Build(n, m);
  }
  void Build(int64_t n, int64_t m) {
    for (int64_t i = size_n - 1; i < size_n + n - 1; i++) {
      for (int64_t j = size_m - 1; j < size_m + m - 1; j++) {
        tree[i][j] = a[i - size_n + 1][j - size_m + 1];
      }
    }
    for (int64_t i = 2 * size_n - 2; i >= size_n - 1; i--) {
      for (int64_t j = size_m - 2; j >= 0; j--) {
        tree[i][j] = std::min(tree[i][LeftM(j)], tree[i][RightM(j)]);
      }
    }
    for (int64_t j = 2 * size_m - 2; j >= size_m - 1; j--) {
      for (int64_t i = size_n - 2; i >= 0; i--) {
        tree[i][j] = std::min(tree[LeftN(i)][j], tree[RightN(i)][j]);
      }
    }
    for (int64_t i = size_n - 2; i >= 0; i--) {
      for (int64_t j = size_m - 2; j >= 0; j--) {
        tree[i][j] =
            std::min(std::min(tree[i][LeftM(j)], tree[i][RightM(j)]), std::min(tree[RightN(i)][j], tree[LeftN(i)][j]));
      }
    }
  }
  int64_t LeftM(int64_t i) {
    if (2 * i + 1 < 2 * size_m - 1) {
      return 2 * i + 1;
    }
    return kNeutralElement;
  }
  int64_t RightM(int64_t i) {
    if (2 * i + 2 < 2 * size_m - 1) {
      return 2 * i + 2;
    }
    return kNeutralElement;
  }
  int64_t LeftN(int64_t i) {
    if (2 * i + 1 < 2 * size_n - 1) {
      return 2 * i + 1;
    }
    return kNeutralElement;
  }
  int64_t RightN(int64_t i) {
    if (2 * i + 2 < 2 * size_n - 1) {
      return 2 * i + 2;
    }
    return kNeutralElement;
  }
  bool Intersect(int64_t l_x, int64_t r_x, int64_t l_y, int64_t r_y, int64_t a, int64_t b, int64_t c, int64_t d) {
    return (r_x < a || l_x > b || r_y < c || l_y > d);
  }
  int64_t QueryforMin(int64_t l_x, int64_t r_x, int64_t l_y, int64_t r_y, int64_t x, int64_t y, int64_t a, int64_t b,
                      int64_t c, int64_t d) {
    if (Intersect(l_x, r_x, l_y, r_y, a, b, c, d)) {
      return kNeutralElement;
    }
    if (a >= l_x && r_x >= b) {
      if (c >= l_y && r_y >= d) {
        return tree[x][y];
      }
      return std::min(QueryforMin(l_x, r_x, l_y, r_y, x, LeftM(y), a, b, c, (c + d) / 2),
                      QueryforMin(l_x, r_x, l_y, r_y, x, RightM(y), a, b, (c + d) / 2 + 1, d));
    }
    if (c >= l_y && r_y >= d) {
      return std::min(QueryforMin(l_x, r_x, l_y, r_y, LeftN(x), y, a, (a + b) / 2, c, d),
                      QueryforMin(l_x, r_x, l_y, r_y, RightN(x), y, (a + b) / 2 + 1, b, c, d));
    }
    return std::min(
        std::min(QueryforMin(l_x, r_x, l_y, r_y, LeftN(x), LeftM(y), a, (a + b) / 2, c, (c + d) / 2),
                 QueryforMin(l_x, r_x, l_y, r_y, RightN(x), LeftM(y), (a + b) / 2 + 1, b, c, (c + d) / 2)),
        std::min(QueryforMin(l_x, r_x, l_y, r_y, LeftN(x), RightM(y), a, (a + b) / 2, (c + d) / 2 + 1, d),
                 QueryforMin(l_x, r_x, l_y, r_y, RightN(x), RightM(y), (a + b) / 2 + 1, b, (c + d) / 2 + 1, d)));
  }
  void Print() {
    for (int64_t i = 0; i < size_n; i++) {
      for (int64_t j = 0; j < size_m; j++) {
        std::cout << tree[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int64_t m = 0;
  int64_t n = 0;
  std::cin >> m >> n;
  std::vector<std::vector<int64_t>> matrix(m, std::vector<int64_t>(n));
  for (int64_t i = 0; i < m; i++) {
    for (int64_t j = 0; j < n; j++) {
      std::cin >> matrix[i][j];
    }
  }
  SegmentTree tree(matrix, m, n);
  // tree.Print();
  int64_t q = 0;
  int64_t x1 = 0;
  int64_t x2 = 0;
  int64_t y1 = 0;
  int64_t y2 = 0;
  std::cin >> q;
  for (int64_t i = 0; i < q; i++) {
    std::cin >> x1 >> y1 >> x2 >> y2;
    x1 += tree.size_n - 2;
    x2 += tree.size_n - 2;
    y1 += tree.size_m - 2;
    y2 += tree.size_m - 2;
    std::cout << tree.QueryforMin(x1, x2, y1, y2, 0, 0, tree.size_n - 1, 2 * tree.size_n - 2, tree.size_m - 1,
                                  2 * tree.size_m - 2)
              << std::endl;
  }
}