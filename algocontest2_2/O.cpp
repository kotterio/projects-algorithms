#include <vector>
#include <iostream>
#include <cmath>

struct Bracket {
  int64_t open = 0;
  int64_t closed = 0;
  int64_t right_sequence = 0;
  Bracket() {
    open = 0;
    closed = 0;
    right_sequence = 0;
  };
};

struct SegmentTree {
  std::vector<Bracket> a;
  std::vector<Bracket> tree;
  SegmentTree() = default;
  int64_t Nextpoweroftwo(int64_t n) {
    if (n == 0) {
      return 1;
    }
    return static_cast<int64_t>(std::pow(2, std::ceil(std::log2(n))));
  }
  explicit SegmentTree(const std::vector<Bracket>& b) : a(b) {
    a.resize(Nextpoweroftwo(static_cast<int>(a.size())), Bracket());
    tree.resize(2 * a.size() - 1);
    Build();
  }
  void Build() {
    for (int64_t i = static_cast<int64_t>(a.size()) - 1; i < static_cast<int64_t>(tree.size()); i++) {
      tree[i] = a[i - a.size() + 1];
    }
    for (int64_t i = static_cast<int64_t>(a.size()) - 2; i >= 0; i--) {
      int64_t size_of_sequence = std::min(tree[2 * i + 1].open, tree[2 * i + 2].closed);
      tree[i].closed = tree[2 * i + 1].closed + tree[2 * i + 2].closed - size_of_sequence;
      tree[i].open = tree[2 * i + 1].open + tree[2 * i + 2].open - size_of_sequence;
      tree[i].right_sequence = tree[2 * i + 1].right_sequence + tree[2 * i + 2].right_sequence + size_of_sequence;
    }
  }
  bool Intersect(int64_t l, int64_t r, int64_t a, int64_t b) {
    return (l > b || r < a);
  }
  bool Contains(int64_t l, int64_t r, int64_t a, int64_t b) {
    return l <= a && b <= r;
  }
  Bracket QueryForSequence(int64_t l, int64_t r, int64_t node, int64_t a, int64_t b) {
    if (Contains(l, r, a, b)) {
      return tree[node];
    }
    if (Intersect(l, r, a, b)) {
      return {};
    }
    Bracket res_left = QueryForSequence(l, r, 2 * node + 1, a, (a + b) / 2);
    Bracket res_right = QueryForSequence(l, r, 2 * node + 2, (a + b) / 2 + 1, b);
    Bracket res;
    int64_t size_of_sequence = std::min(res_left.open, res_right.closed);
    res.open = res_left.open + res_right.open - size_of_sequence;
    res.closed = res_left.closed + res_right.closed - size_of_sequence;
    res.right_sequence = res_left.right_sequence + res_right.right_sequence + size_of_sequence;
    return res;
  }
};

int main() {
  std::string s;
  std::cin >> s;
  std::vector<Bracket> a(s.size());
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i] == '(') {
      a[i].open = 1;
    } else if (s[i] == ')') {
      a[i].closed = 1;
    }
  }
  SegmentTree tree(a);
  int64_t q = 0;
  std::cin >> q;
  int64_t l = 0;
  int64_t r = 0;
  int64_t n = 0;
  n = tree.Nextpoweroftwo(static_cast<int64_t>(s.size()));
  for (int64_t i = 0; i < q; i++) {
    std::cin >> l;
    std::cin >> r;
    l += n - 2;
    r += n - 2;
    std::cout << tree.QueryForSequence(l, r, 0, n - 1, 2 * n - 2).right_sequence * 2 << std::endl;
  }
}