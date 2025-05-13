#include <random>
#include <iostream>
#include <memory>

struct Node {
  int64_t x, y, size, min;
  bool reverse;
  Node* l;
  Node* r;
  explicit Node(int64_t value) {
    x = value;
    y = rand();
    l = nullptr;
    r = nullptr;
    size = 1;
    reverse = false;
    min = value;
  }
};

class Tree {
 public:
  Node* root;
  Tree() : root(nullptr) {
  }
  ~Tree() {
    if (root == nullptr) {
      return;
    }
    Delete(root);
  }
  void Delete(Node* node) {
    if (node == nullptr) {
      return;
    }
    Delete(node->l);
    Delete(node->r);
    delete node;
  }
  int64_t Size(Node* t) {
    return t ? t->size : 0;
  }
  int64_t Min(Node* t) {
    return t ? t->min : INT64_MAX;
  }
  void UpdateSizeMinimum(Node*& node) {
    if (node) {
      node->size = 1 + Size(node->l) + Size(node->r);
      node->min = std::min(std::min(Min(node->l), node->x), Min(node->r));
    }
  }
  void Insertwithx(int64_t ind, int64_t value) {
    if (!Existforroot(value)) {
      Insert(ind, value);
    }
  }
  void Insert(int64_t ind, int64_t x) {
    if (root) {
      root->size++;
    }
    std::pair<Node*, Node*> pair = Split(root, ind);
    Node* node = new Node(x);
    Node* t_0 = Merge(pair.first, node);
    root = Merge(t_0, pair.second);
  }
  std::pair<Node*, Node*> Split(Node*& t, int64_t k) {
    if (t == nullptr) {
      return std::make_pair(nullptr, nullptr);
    }
    std::pair<Node*, Node*> pair;
    Push(t);
    if (Size(t->l) < k) {
      pair = Split(t->r, k - Size(t->l) - 1);
      t->r = pair.first;
      UpdateSizeMinimum(t);
      return {t, pair.second};
    }
    pair = Split(t->l, k);
    t->l = pair.second;
    UpdateSizeMinimum(t);
    return {pair.first, t};
  }
  Node* Merge(Node*& t1, Node*& t2) {
    if (t1 == nullptr || t2 == nullptr) {
      return t1 ? t1 : t2;
    }
    if (t1->y > t2->y) {
      Push(t2);
      t2->l = Merge(t1, t2->l);
      UpdateSizeMinimum(t2);
      return t2;
    }
    Push(t1);
    t1->r = Merge(t1->r, t2);
    UpdateSizeMinimum(t1);
    return t1;
  }
  bool Existforroot(int64_t value) {
    if (root == nullptr) {
      return false;
    }
    if (root->x == value) {
      return true;
    }
    return Exist(root, value);
  }
  bool Exist(Node*& t, int64_t value) {
    if (t) {
      if (t->x == value) {
        return true;
      }
      if (value > t->x) {
        return Exist(t->r, value);
      }
      return Exist(t->l, value);
    }
    return false;
  }
  void Push(Node*& node) {
    if (node && node->reverse) {
      node->reverse = false;
      std::swap(node->l, node->r);
      if (node->l) {
        node->l->reverse ^= true;
      }
      if (node->r) {
        node->r->reverse ^= true;
      }
    }
  }
  void Reverse(int64_t left, int64_t right) {
    std::pair<Node*, Node*> t0_t1 = Split(root, left);
    std::pair<Node*, Node*> t2_t3 = Split(t0_t1.second, right - left + 1);
    t2_t3.first->reverse ^= true;
    root = Merge(t0_t1.first, t2_t3.first);
    root = Merge(root, t2_t3.second);
  }

  int64_t FindMin(int64_t left, int64_t right) {
    std::pair<Node*, Node*> pair_first = Split(root, left);
    std::pair<Node*, Node*> pair_second = Split(pair_first.second, right - left + 1);
    int64_t min = Min(pair_second.first);
    pair_first.second = Merge(pair_second.first, pair_second.second);
    root = Merge(pair_first.first, pair_first.second);
    return min;
  }
};

int main() {
  Tree tree;
  std::ios::sync_with_stdio(false);
  std::cout.tie(nullptr);
  int64_t n = 0;
  std::cin >> n;
  int64_t m = 0;
  std::cin >> m;
  int64_t num = 0;
  for (int64_t i = 0; i < n; i++) {
    std::cin >> num;
    tree.Insert(i, num);
  }
  int64_t id = 0;
  int64_t l = 0;
  int64_t r = 0;
  for (int64_t i = 0; i < m; i++) {
    std::cin >> id;
    if (id == 1) {
      std::cin >> l;
      std::cin >> r;
      l--;
      r--;
      tree.Reverse(l, r);
    } else {
      std::cin >> l;
      std::cin >> r;
      l--;
      r--;
      std::cout << tree.FindMin(l, r) << std::endl;
    }
  }
}