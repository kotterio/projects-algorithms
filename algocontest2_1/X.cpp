#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <string>
#include <algorithm>

const int kKalphabet = 28;

struct Node;

class SuffAuto {
 public:
  int GetIndex(char c) {
    if (c >= 'a' && c <= 'z') {
      return c - 'a';
    }
    return c == '#' ? 26 : 27;
  }
  struct Node {
    size_t len = 0;
    int64_t link = -1;
    int64_t end_pos = -1;
    std::vector<int64_t> to;
    Node() {
      to.assign(kKalphabet + 2, -1);
    }
  };
  int64_t last = 0;
  std::vector<Node> t;
  SuffAuto() {
    t.emplace_back();
  }
  explicit SuffAuto(const std::string& s) {
    t.emplace_back();
    for (auto c : s) {
      Add(c);
    }
  }
  void Addtodes(const std::string& s) {
    for (auto c : s) {
      Add(c);
    }
  }
  void Add(const char& c) {
    t.emplace_back();
    int64_t curr = static_cast<int64_t>(t.size()) - 1;
    int64_t p = last;
    while (p != -1 && t[p].to[GetIndex(c)] == -1) {
      t[p].to[GetIndex(c)] = curr;
      p = t[p].link;
    }
    t[curr].len = t[last].len + 1;
    t[curr].end_pos = t[last].end_pos + 1;
    if (p == -1) {
      t[curr].link = 0;
      last = curr;
      return;
    }
    auto q = t[p].to[GetIndex(c)];
    if (t[q].len == t[p].len + 1) {
      t[curr].link = q;
      last = curr;
      return;
    }
    t.emplace_back();
    int64_t clone = static_cast<int64_t>(t.size()) - 1;
    t[clone].len = t[p].len + 1;
    t[clone].end_pos = t[q].end_pos;
    while (p != -1 && t[p].to[GetIndex(c)] == q) {
      t[p].to[GetIndex(c)] = clone;
      p = t[p].link;
    }
    t[clone].to = t[q].to;
    t[clone].link = t[q].link;
    t[q].link = clone;
    t[curr].link = clone;
    last = curr;
  }
  bool Desicion(const std::string& st) {
    int64_t curr = last;
    size_t len_st = static_cast<int64_t>(st.length());
    return t[t[curr].link].len < len_st;
  }
};

class CT {
 public:
  struct NodeSt {
    std::vector<int64_t> to;
    size_t begin = 0;
    size_t length = 0;
    NodeSt() {
      to.assign(kKalphabet + 2, -1);
    }
  };
  int GetIndex(char c) {
    if (c >= 'a' && c <= 'z') {
      return c - 'a';
    }
    return c == '#' ? 26 : 27;
  }
  std::vector<NodeSt> nodes;
  std::string st;
  size_t k = 0;
  void ExtractTransitions(const SuffAuto& suff, int64_t node_id, size_t more) {
    int64_t suff_id = suff.t[node_id].link;
    nodes[node_id].begin = st.size() - 1 - (suff.t[node_id].end_pos - suff.t[suff_id].len);
    if ((suff.t[node_id].len - suff.t[suff_id].len) > more) {
      nodes[node_id].length = (suff.t[node_id].len - suff.t[suff_id].len) - more;
    } else {
      nodes[node_id].length = suff.t[node_id].len - suff.t[suff_id].len;
    }
    if (nodes[node_id].length == 0) {
      k++;
      return;
    }
    nodes[suff_id].to[GetIndex(st[nodes[node_id].begin])] = node_id;
  }
  CT(const SuffAuto& suff, const std::string& str, size_t more) {
    nodes = std::vector<NodeSt>(suff.t.size());
    st = str;
    for (size_t i = 1; i < suff.t.size(); i++) {
      ExtractTransitions(suff, static_cast<int64_t>(i), more);
    }
  }
  void Print() {
    std::cout << nodes.size() - k << std::endl;
    for (size_t i = 0; i < nodes.size(); i++) {
      for (size_t j = 0; j < nodes[i].to.size(); j++) {
        if (nodes[i].to[j] != -1) {
          int64_t id = nodes[i].to[j];
          std::cout << i << " " << st.substr(nodes[id].begin, nodes[id].length) << " " << nodes[id].length << " " << id << std::endl;
        }
      }
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::string first_stroka;
  std::cin >> first_stroka;
  std::string right_first_stroka = first_stroka;
  std::string second_stroka;
  std::cin >> second_stroka;
  std::string right_secondt_stroka = second_stroka;
  std::string to_suf = right_first_stroka + right_secondt_stroka;
  size_t c = right_secondt_stroka.length();
  std::reverse(to_suf.begin(), to_suf.end());
  SuffAuto suff1(to_suf);
  CT cts(suff1, right_first_stroka + right_secondt_stroka, c);
  cts.Print();
}