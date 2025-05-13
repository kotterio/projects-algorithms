#include <random>
#include <iostream>
#include <memory>

struct Node {
  int x, y, size;
  Node* l;
  Node* r;
  explicit Node(int value) {
    x = value;
    y = rand();
    l = nullptr;
    r = nullptr;
    size = 1;
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
  void Kth(int k) {
    if (root == nullptr || Size(root) < k + 1 || k < 0) {
      std::cout << "none" << std::endl;
      return;
    }
    std::cout << FindKthMax(root, k) << std::endl;
  }
  int FindKthMax(const Node* t, int k) {
    if (k == 0 && !t->l) {
      return t->x;
    }
    if (k < Size(t->l)) {
      return FindKthMax(t->l, k);
    }
    if (k == Size(t->l)) {
      return t->x;
    }
    return FindKthMax(t->r, k - Size(t->l) - 1);
  }
  void Insertwithx(int value) {
    if (!Existforroot(value)) {
      Insert(value);
    }
  }
  void Insert(int x) {
    std::pair<Node*, Node*> pair = Split(root, x);
    Node* node = new Node(x);
    Node* t_0 = Merge(pair.first, node);
    root = Merge(t_0, pair.second);
  }
  int Size(const Node* t) {
    return t != nullptr ? t->size : 0;
  }
  void UpdateSize(Node*& t) {
    if (t != nullptr) {
      t->size = 1 + Size(t->l) + Size(t->r);
    }
  }
  std::pair<Node*, Node*> Split(Node*& t, int k) {
    if (t == nullptr) {
      return std::make_pair(nullptr, nullptr);
    }
    std::pair<Node*, Node*> pair;
    if (t->x < k) {
      pair = Split(t->r, k);
      t->r = pair.first;
      UpdateSize(t);
      return {t, pair.second};
    }
    pair = Split(t->l, k);
    t->l = pair.second;
    UpdateSize(t);
    return {pair.first, t};
  }
  Node* Merge(Node*& t1, Node*& t2) {
    if (t1 == nullptr || t2 == nullptr) {
      return t1 ? t1 : t2;
    }
    if (t1->y > t2->y) {
      t2->l = Merge(t1, t2->l);
      UpdateSize(t2);
      return t2;
    }
    t1->r = Merge(t1->r, t2);
    UpdateSize(t1);
    return t1;
  }
  void Next(int n) {
    std::pair<Node*, Node*> pair = Split(root, n + 1);
    Node* node = pair.second;
    if (!node) {
      std::cout << "none" << std::endl;
      return;
    }
    while (node->l) {
      node = node->l;
    }
    root = Merge(pair.first, pair.second);
    std::cout << node->x << std::endl;
  }
  void Prev(int n) {
    std::pair<Node*, Node*> pair = Split(root, n);
    Node* node = pair.first;
    if (!node) {
      std::cout << "none" << std::endl;
      return;
    }
    while (node->r) {
      node = node->r;
    }
    root = Merge(pair.first, pair.second);
    std::cout << node->x << std::endl;
  }
  bool Existforroot(int value) {
    if (root == nullptr) {
      return false;
    }
    if (root->x == value) {
      return true;
    }
    return Exist(root, value);
  }
  bool Exist(Node*& t, int value) {
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
  void Erase(int value) {
    if (Existforroot(value)) {
      std::pair<Node*, Node*> t1_t0 = Split(root, value);
      std::pair<Node*, Node*> t00_t2 = Split(t1_t0.second, value + 1);
      Delete(t00_t2.first);
      root = Merge(t1_t0.first, t00_t2.second);
    }
  }
};

int main() {
  Tree tree;
  std::ios::sync_with_stdio(false);
  std::cout.tie(nullptr);
  std::string s;
  int n = 0;
  while (std::cin >> s) {
    std::cin >> n;
    if (s == "insert") {
      tree.Insert(n);
    } else if (s == "delete") {
      tree.Erase(n);
    } else if (s == "exists") {
      if (tree.Existforroot(n)) {
        std::cout << "true" << std::endl;
      } else {
        std::cout << "false" << std::endl;
      }
    } else if (s == "next") {
      tree.Next(n);
    } else if (s == "prev") {
      tree.Prev(n);
    } else if (s == "kth") {
      tree.Kth(n);
    }
  }
  return 0;
}