#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <memory>
#include <cmath>
#include <map>

class BorNode;
using LinksMap = std::map<char, std::shared_ptr<BorNode>>;

#define size_t int64_t

class BorNode {
 public:
  LinksMap links;
  std::weak_ptr<BorNode> suffix;
  std::weak_ptr<BorNode> term_suffix;
  size_t out;
  int64_t lost;
  int64_t prev;
  uint8_t color = 0;
  std::string st;
  BorNode() : out(1000000) {
  }
  std::shared_ptr<BorNode> Getlink(char c) const {
    auto iter = links.find(c);
    if (iter != links.cend()) {
      return iter->second;
    }
    return nullptr;
  }
  bool IsTerminal() const {
    return (out != 1000000);
  }
};

class AhoCorasik {
 public:
  std::vector<std::pair<std::string, std::vector<size_t>>> dict;
  std::shared_ptr<BorNode> root;
  std::map<std::string, std::pair<std::weak_ptr<BorNode>, std::string>> words;
  AhoCorasik() : root(std::make_shared<BorNode>()) {
  }
  void AddString(const std::string& str, const std::string& text, size_t m) {
    std::shared_ptr<BorNode> current_node = root;
    size_t k = m;
    for (auto c : str) {
      std::shared_ptr<BorNode> child_node = current_node->Getlink(c);
      current_node->lost = static_cast<int64_t>(std::pow(10ll, k));
      current_node->prev = current_node->lost;
      if (!child_node) {
        child_node = std::make_shared<BorNode>();
        current_node->links[c] = child_node;
      }
      current_node = child_node;
      k--;
    }
    current_node->st = str;
    current_node->out = static_cast<int64_t>(words.size());
    if (m == static_cast<int64_t>(str.length())) {
      current_node->lost = 1;
    } else {
      current_node->lost = static_cast<int64_t>(std::pow(10ll, m - str.length()));
    }
    current_node->prev = current_node->lost;
    words[text].first = current_node;
    words[text].second = str;
  }
  void DFS(std::shared_ptr<BorNode> root, size_t m) {
    std::weak_ptr<BorNode> wroot = root;
    for (auto link : root->links) {
      if (link.second->color == 0) {
        DfsVisit(link.second, wroot, m);
      }
    }
  }
  void DfsVisit(std::shared_ptr<BorNode> node, std::weak_ptr<BorNode> parent, size_t m) {
    node->color = 1;
    std::weak_ptr<BorNode> wnode = node;
    for (auto link : node->links) {
      if (link.second->color == 0) {
        DfsVisit(link.second, wnode, m);
      }
    }
    std::shared_ptr<BorNode> p = parent.lock();
    if (node->IsTerminal()) {
      p->lost -= node->prev;
    } else {
      p->lost -= (static_cast<int64_t>(node->prev - node->lost));
    }
    node->color = 2;
  }
};

int main() {
  AhoCorasik ah;
  std::string text;
  size_t n = 0;
  std::cin >> n;
  std::string word;
  size_t m = 0;
  std::cin >> m;
  for (size_t i = 0; i < n; i++) {
    std::cin >> word;
    std::cin >> text;
    if (static_cast<int64_t>(word.length()) > m) {
      std::cout << text << " " << 0 << std::endl;
      continue;
    }
    ah.AddString(word, text, m);
  }
  ah.DFS(ah.root, m);
  for (auto elem : ah.words) {
    std::shared_ptr<BorNode> node = elem.second.first.lock();
    std::cout << elem.first << " " << node->lost << std::endl;
  }
}
