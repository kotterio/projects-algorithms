#include <vector>
#include <iostream>
#include <cmath>

class SegmentTree {
 public:
  explicit SegmentTree(std::vector<int64_t> &a) {
    sz_ = static_cast<int64_t>(a.size());
    tree_.resize(2 * a.size() - 1);
    for (int64_t i = static_cast<int64_t>(a.size()) - 1; i < static_cast<int64_t>(tree_.size()); ++i) {
      tree_[i].first = a[i - a.size() + 1];
      tree_[i].second = 1;
    }
    for (int64_t i = static_cast<int64_t>(a.size()) - 2; i >= 0; --i) {
      if (tree_[2 * i + 1].first == tree_[2 * i + 2].first) {
        tree_[i].first = tree_[2 * i + 1].first;
        tree_[i].second = tree_[2 * i + 1].second + tree_[2 * i + 2].second;
      } else if (tree_[2 * i + 1].first > tree_[2 * i + 2].first) {
        tree_[i].first = tree_[2 * i + 1].first;
        tree_[i].second = tree_[2 * i + 1].second;
      } else {
        tree_[i].first = tree_[2 * i + 2].first;
        tree_[i].second = tree_[2 * i + 2].second;
      }
    }
  }
  std::pair<int64_t, int64_t> Max(int64_t l, int64_t r) {
    l += sz_ - 1;
    r += sz_ - 1;
    int64_t resultl = 0;
    int64_t countresultr = 0;
    int64_t countresultl = 0;
    int64_t resultr = 0;
    if (l == r) {
      return tree_[l];
    }
    while (l < r) {
      if (l % 2 == 0) {
        if (resultl < tree_[l].first) {
          resultl = tree_[l].first;
          countresultl = tree_[l].second;
        } else if (resultl == tree_[l].first) {
          countresultl += tree_[l].second;
        }
      }
      l /= 2;
      if (r % 2 == 1) {
        if (resultr < tree_[r].first) {
          resultr = tree_[r].first;
          countresultr = tree_[r].second;
        } else if (resultr == tree_[r].first) {
          countresultr += tree_[r].second;
        }
      }
      r = r / 2 - 1;
      if (l == r) {
        if (resultl < tree_[l].first) {
          resultl = tree_[l].first;
          countresultl = tree_[l].second;
        } else if (resultl == tree_[l].first) {
          countresultl += tree_[l].second;
        }
      }
    }
    if (resultl > resultr) {
      return {resultl, countresultl};
    }
    if (resultl < resultr) {
      return {resultr, countresultr};
    }
    return {resultr, countresultl + countresultr};
  }

 private:
  std::vector<std::pair<int64_t, int64_t>> tree_;
  int64_t sz_;
};

int64_t Nextpoweroftwo(int64_t n) {
  if (n == 0) {
    return 1;
  }
  return static_cast<int64_t>(std::pow(2, std::ceil(std::log2(n))));
}

int main() {
  int64_t n = 0;
  std::cin >> n;
  int64_t power = Nextpoweroftwo(n);
  std::vector<int64_t> a(power, 0);
  for (int64_t i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  SegmentTree tree(a);
  size_t k = 0;
  std::cin >> k;
  int64_t l = 0;
  int64_t r = 0;
  for (size_t i = 0; i < k; i++) {
    std::cin >> l;
    std::cin >> r;
    std::pair<int64_t, int64_t> res = tree.Max(l - 1, r - 1);
    std::cout << res.first << " " << res.second << std::endl;
  }
}