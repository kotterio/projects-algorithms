#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <array>
#include <memory>
#include <string>

class BorNode;
using LinksMap = std::map<char, std::shared_ptr<BorNode>>;

class BorNode {
 public:
  LinksMap links;
  std::weak_ptr<BorNode> suffix;
  std::weak_ptr<BorNode> term_suffix;
  size_t out;
  BorNode() : out(1000000) {
  }
  std::shared_ptr<BorNode> Getlink(const char c) const {
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

struct Special {
  std::shared_ptr<BorNode> node;
  std::weak_ptr<BorNode> parrent;
  const char symbol;
  Special(std::pair<const char, std::shared_ptr<BorNode>> elem, std::shared_ptr<BorNode> n)
      : node(elem.second), parrent(n), symbol(elem.first){};
};

class AhoCorasik {
 public:
  std::vector<std::pair<std::string, std::vector<size_t>>> dict;
  std::shared_ptr<BorNode> root;
  std::vector<std::string> words;
  AhoCorasik() : root(std::make_shared<BorNode>()) {
  }
  void AddString(const std::string& str) {
    std::shared_ptr<BorNode> current_node = root;
    for (const char c : str) {
      std::shared_ptr<BorNode> child_node = current_node->Getlink(c);
      if (!child_node) {
        child_node = std::make_shared<BorNode>();
        current_node->links[c] = child_node;
      }
      current_node = child_node;
    }
    current_node->out = words.size();
    words.emplace_back(str);
  }
  void BuildSuffixRefernces() {
    root->suffix.lock() = nullptr;
    std::queue<Special> q;
    for (auto elem : root->links) {
      Special s(elem, root);
      q.push(s);
    }
    while (!q.empty()) {
      Special cur = q.front();
      q.pop();
      std::shared_ptr<BorNode> suf = cur.parrent.lock()->suffix.lock();
      while (suf != nullptr && suf->Getlink(cur.symbol) == nullptr) {
        suf = suf->suffix.lock();
      }
      cur.node->suffix = (suf == nullptr ? root : suf->Getlink(cur.symbol));
      for (auto elem : cur.node->links) {
        Special s(elem, cur.node);
        q.push(s);
      }
    }
  }
  void BuildExitLinks() {
    root->term_suffix.lock() = nullptr;
    std::queue<std::shared_ptr<BorNode>> q;
    for (auto elem : root->links) {
      q.push(elem.second);
    }
    while (!q.empty()) {
      std::shared_ptr<BorNode> node = q.front();
      q.pop();
      std::shared_ptr<BorNode> suf = node->suffix.lock();
      if (suf && suf.get() != root.get()) {
        node->term_suffix = (suf->IsTerminal() ? suf : suf->term_suffix);
      }
      for (auto neigh : node->links) {
        q.push(neigh.second);
      }
    }
  }
  std::shared_ptr<BorNode> NextState(std::shared_ptr<BorNode> node, char s) {
    while (node != nullptr && node->Getlink(s) == nullptr) {
      node = node->suffix.lock();
    }
    return node == nullptr ? root : node->Getlink(s);
  }
  std::pair<size_t, bool> Print(size_t index, std::shared_ptr<BorNode> node, size_t n) {
    size_t colvo = 0;
    bool flag = false;
    std::shared_ptr<BorNode> current_node = node;
    if (!current_node->IsTerminal()) {
      current_node = current_node->term_suffix.lock();
    }
    while (current_node != nullptr) {
      if (index - words[current_node->out].length() + 1 == n) {
        flag = true;
        dict[current_node->out].second.push_back(index - words[current_node->out].length() + 1);
        if (colvo < index) {
          colvo = index;
        }
      }
      current_node = current_node->term_suffix.lock();
    }
    std::pair<size_t, bool> result;
    result.first = colvo;
    result.second = flag;
    return result;
  }
};

struct Help {
  std::vector<std::shared_ptr<Help>> next;
  uint16_t color;
  std::string st_now;
  Help() : color(0) {
  }
  explicit Help(std::string& st) : color(0), st_now(st) {
  }
};

struct Desicion {
  std::shared_ptr<Help> root;
  std::string result;
  bool flagforres;
  Desicion(std::string& str, AhoCorasik& ah) : root(std::make_shared<Help>()), flagforres(false) {
    std::shared_ptr<BorNode> node = ah.root;
    size_t colvo = 0;
    std::pair<size_t, bool> max_id;
    bool flag = false;
    for (size_t i = 0; i < str.length(); i++) {
      node = ah.NextState(node, str[i]);
      max_id = ah.Print(i, node, 0);
      if (colvo < max_id.first) {
        colvo = max_id.first;
      }
      if (!flag) {
        flag = max_id.second;
      }
    }
    if (flag) {
      root->next = std::vector<std::shared_ptr<Help>>(colvo + 1);
      for (size_t i = 0; i <= colvo; i++) {
        std::string otvet = ah.words[colvo - i];
        std::shared_ptr<Help> child = std::make_shared<Help>(ah.words[colvo - i]);
        std::string withoutpref = str.substr(ah.words[colvo - i].length());
        root->next[i] = child;
        HelpForConstructor(child, ah, withoutpref);
      }
    }
  }
  void HelpForConstructor(std::shared_ptr<Help> parent, AhoCorasik& ah, std::string& withoutpref) {
    std::shared_ptr<BorNode> node = ah.root;
    std::string safestr = withoutpref;
    size_t colvo = 0;
    std::pair<size_t, bool> max_id;
    bool flag = false;
    for (size_t i = 0; i < withoutpref.length(); i++) {
      node = ah.NextState(node, withoutpref[i]);
      max_id = ah.Print(i, node, 0);
      if (colvo < max_id.first) {
        colvo = max_id.first;
      }
      if (!flag) {
        flag = max_id.second;
      }
    }
    if (!flag) {
      return;
    }
    parent->next = std::vector<std::shared_ptr<Help>>(colvo + 1);
    for (size_t i = 0; i <= colvo; i++) {
      std::shared_ptr<Help> child = std::make_shared<Help>(ah.words[colvo - i]);
      std::string withoutpref = safestr.substr(ah.words[colvo - i].length());
      parent->next[i] = child;
      HelpForConstructor(child, ah, withoutpref);
    }
  }
  void Print(std::string& str) {
    std::string otvet;
    for (size_t i = 0; i < root->next.size(); i++) {
      std::string otvet = root->next[i]->st_now;
      if (otvet == str) {
        std::cout << "No" << std::endl;
        std::cout << otvet << std::endl;
        flagforres = true;
        return;
      }
      Dfs(str, root->next[i], otvet, otvet);
    }
  }
  void Dfs(const std::string& str, std::shared_ptr<Help> help_node, std::string otvet, std::string otvetcin) {
    help_node->color = 1;
    if (flagforres) {
      return;
    }
    for (size_t i = 0; i < help_node->next.size(); i++) {
      if (help_node->next[i]->color == 0) {
        if (otvet + help_node->next[i]->st_now == str) {
          std::cout << "No" << std::endl;
          std::cout << otvetcin + " " + help_node->next[i]->st_now << std::endl;
          flagforres = true;
          return;
        }
        Dfs(str, help_node->next[i], otvet + help_node->next[i]->st_now, otvetcin + " " + help_node->next[i]->st_now);
      }
    }
    help_node->color = 2;
  }
};

int main() {
  AhoCorasik ah;
  std::string text;
  std::cin >> text;
  size_t n = text.length();
  std::string str;
  std::cin >> str;
  std::string word;
  ah.dict = std::vector<std::pair<std::string, std::vector<size_t>>>(n);
  for (size_t i = 0; i < n; i++) {
    word = text.substr(0, i + 1);
    ah.dict[i].first = word;
    ah.AddString(word);
  }
  ah.BuildSuffixRefernces();
  ah.BuildExitLinks();
  std::shared_ptr<BorNode> node = ah.root;
  Desicion des(str, ah);
  des.Print(str);
  if (!des.flagforres) {
    std::cout << "Yes";
  }
}