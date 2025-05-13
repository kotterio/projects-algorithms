#include <iostream>
#include <vector>
#include <cmath>
#include <string>

struct Node {
  int64_t value = 0;
  int64_t prior = 0;
  int64_t size = 0;
  Node* parent = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;

  explicit Node(int64_t n) {
    value = n;
    prior = rand();
    size = 1;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
  }

  Node(int64_t n, int64_t y) {
    value = n;
    prior = y;
    size = 1;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
  }

  Node(int64_t n, int64_t y, Node* parent_new, Node* left_new, Node* right_new) {
    value = n;
    prior = y;
    size = 1;
    parent = parent_new;
    left = left_new;
    right = right_new;
  }
};

struct Treap {
  Node* root = nullptr;
  int64_t size = 0;

  int64_t Size(Node* t) const {
    return (t ? t->size : 0);
  }

  void FixNode(Node* trip) {
    if (trip) {
      trip->size = 1 + Size(trip->left) + Size(trip->right);
    }
  }

  Node* Merge(Node* t_1, Node* t_2) {
    if (t_1 == nullptr) {
      return t_2;
    }
    if (t_2 == nullptr) {
      return t_1;
    }
    if (t_1->prior > t_2->prior) {
      t_2->left = Merge(t_1, t_2->left);
      t_2->left->parent = t_2;
      FixNode(t_2);
      return t_2;
    }
    t_1->right = Merge(t_1->right, t_2);
    t_1->right->parent = t_1;
    FixNode(t_1);
    return t_1;
  }

  std::pair<Node*, Node*> Split(Node* trip, int64_t n) {
    if (trip == nullptr) {
      return std::make_pair(nullptr, nullptr);
    }
    std::pair<Node*, Node*> pair;
    if (trip->value < n) {
      pair = Split(trip->right, n);
      trip->right = pair.first;
      if (pair.first) {
        pair.first->parent = trip;
      }
      if (pair.second) {
        pair.second->parent = nullptr;
      }
      FixNode(trip);
      FixNode(pair.second);
      return std::make_pair(trip, pair.second);
    }
    pair = Split(trip->left, n);
    trip->left = pair.second;
    if (pair.first) {
      pair.first->parent = nullptr;
    }
    if (pair.second) {
      pair.second->parent = trip;
    }
    FixNode(pair.first);
    FixNode(trip);
    return std::make_pair(pair.first, trip);
  }

  bool Exists(Node* trip, int64_t n) const {
    if (trip) {
      if (trip->value == n) {
        return true;
      }
      if (n > trip->value) {
        return Exists(trip->right, n);
      }
      return Exists(trip->left, n);
    }
    return false;
  }

  void Insert(int64_t n) {
    if (!Exists(root, n)) {
      size++;
      std::pair<Node*, Node*> pair;
      pair = Split(root, n);
      auto node = new Node(n);
      root = Merge(Merge(pair.first, node), pair.second);
      FixNode(root);
      FixNode(pair.first);
      FixNode(pair.second);
    }
  }

  void Delete(int64_t n) {
    if (Exists(root, n)) {
      size--;
      std::pair<Node*, Node*> pair_1 = Split(root, n);
      std::pair<Node*, Node*> pair_2 = Split(pair_1.second, n + 1);
      delete pair_2.first;
      root = Merge(pair_1.first, pair_2.second);
      FixNode(pair_1.first);
      FixNode(pair_2.second);
      FixNode(root);
    }
  }

  void Next(int64_t n) {
    std::pair<Node*, Node*> pair = Split(root, n + 1);
    Node* cur = pair.second;
    if (!cur) {
      std::cout << "none\n";
      return;
    }
    while (cur->left) {
      cur = cur->left;
    }
    root = Merge(pair.first, pair.second);
    std::cout << cur->value << "\n";
  }

  void Prev(int64_t n) {
    std::pair<Node*, Node*> pair = Split(root, n);
    Node* cur = pair.first;
    if (!cur) {
      std::cout << "none\n";
      return;
    }
    while (cur->right) {
      cur = cur->right;
    }
    root = Merge(pair.first, pair.second);
    std::cout << cur->value << "\n";
  }

  void Kth(int64_t k) {
    if (!root || Size(root) < k + 1) {
      std::cout << "none\n";
      return;
    }
    Kth(root, k);
  }

  void Kth(Node* trip, int64_t k) const {
    if (!k && !trip->left) {
      std::cout << trip->value << "\n";
    } else {
      if (k < Size(trip->left)) {
        Kth(trip->left, k);
      } else if (k == Size(trip->left)) {
        std::cout << trip->value << "\n";
      } else {
        Kth(trip->right, k - Size(trip->left) - 1);
      }
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cout.tie(nullptr);
  std::string s;
  int64_t n = 0;
  Treap trip;
  while (std::cin >> s) {
    std::cin >> n;
    if (s == "insert") {
      trip.Insert(n);
    } else if (s == "delete") {
      trip.Delete(n);
    } else if (s == "exists") {
      std::cout << (trip.Exists(trip.root, n) ? "true\n" : "false\n");
    } else if (s == "next") {
      trip.Next(n);
    } else if (s == "prev") {
      trip.Prev(n);
    } else if (s == "kth") {
      trip.Kth(n);
    }
  }
  return 0;
}