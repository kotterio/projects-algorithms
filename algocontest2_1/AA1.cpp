#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <iterator>

const int kKalphabet = 26;

struct Node;

class SuffAuto {
 public:
  int GetIndex(char c) {
    if (c >= 'a' && c <= 'z') {
      return c - 'a';
    }
    return c + 32 - 'a';
  }
  struct Node {
    int64_t cnt = 0;
    size_t len = 0;
    bool visit = false;
    int64_t link = -1;
    std::vector<int64_t> to;
    int64_t endpos = -1;
    bool visitfromsuf = false;
    Node() {
      to.assign(kKalphabet, -1);
    }
  };
  struct LengthComparator {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
      if (lhs.length() == rhs.length()) {
        return lhs < rhs;
      }
      return lhs.length() < rhs.length();
    }
  };
  int64_t last = 0;
  std::vector<Node> t;
  std::string s;
  std::set<std::string> answer;
  int64_t fullways = 0;
  std::unordered_map<std::string,bool> visit;
  size_t k = 0;
  SuffAuto() {
    t.emplace_back();
  }
  void Print() {
    for (size_t i = 0; i < t.size(); i++) {
      for (int64_t j = 0; j < static_cast<int64_t>(t[i].to.size()); ++j) {
        if (t[i].to[j] != -1) {
          std::cout << i << " " << static_cast<char>('a' + j) << " " << t[i].to[j] << std::endl;
        }
      }
    }
  }
  SuffAuto(const std::string& s, size_t r) {
    k = r;
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
      // std::cout << "not shabge " << curr << " " << p << " " << t[p].cnt << " " << t[curr].cnt << std::endl;
      t[p].to[GetIndex(c)] = curr;
      p = t[p].link;
    }
    // std::cout << " full " << fullways << std::endl;
    t[curr].len = t[last].len + 1;
    t[curr].endpos = t[last].endpos + 1;
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
    t[clone].endpos = t[q].endpos;
    while (p != -1 && t[p].to[GetIndex(c)] == q) {
      // Freecnt(t[p].to[GetIndex(c)], t[p].cnt);
      // Freecnt(t[p].to[GetIndex(c)], t[p].cnt);
      t[p].to[GetIndex(c)] = clone;
      p = t[p].link;
    }
    t[clone].to = t[q].to;
    t[clone].link = t[q].link;
    t[q].link = clone;
    t[curr].link = clone;
    last = curr;
  }

  void Answer(const std::string& query, const std::string& str) {
    int64_t v = 0;
    size_t f = 0;
    bool fromsuf = false;
    for (char c : query) {
      // std::cout << "u " << u << std::endl;
      if (v != -1 && t[v].to[GetIndex(c)] == -1) {
        size_t j = 0;
        while (j != t[v].len) {
          std::string stre = str.substr(t[v].endpos + 1 - t[v].len + j, t[v].len - j);
          if (!visit[str]) {
            if (answer.size() < k) {
              answer.insert(stre);
            } else {
              auto it = answer.rbegin();
              if (*it > stre) {
                answer.erase(*it);
                answer.insert(stre);
              }
            }
          }
        visit[stre] = true;
        f = f + 1;
        // std::cout << f << " next not" << std::endl;
      }
      }
      while (v != -1 && t[v].to[GetIndex(c)] == -1) {
        // std::cout << c << std::endl;
        v = t[v].link;
        fromsuf = true;
      }
      if (v != -1 && t[v].to[GetIndex(c)] != -1) {
        if (fromsuf) {
          if (!t[v].visitfromsuf) {
            size_t j = 0;
            while (j != t[v].len) {
              std::string stre = str.substr(t[v].endpos + 1 - t[v].len + j, t[v].len - j);
              if (!visit[str]) {
                if (answer.size() < k) {
                  answer.insert(stre);
                } else {
                  auto it = answer.rbegin();
                  if (*it > stre) {
                    answer.erase(*it);
                    answer.insert(stre);
                  }
                }
              }
              visit[stre] = true;
              j++;
            }
            t[v].visitfromsuf = false;
          }
          // std::cout << "from suf " << f << " " << t[v].len + 1 << std::endl;
          f = t[v].len + 1;
          v = t[v].to[GetIndex(c)];
          fromsuf = false;
          continue;
        }
        if (!fromsuf) {
          size_t j = 0;
          // std::cout << "from mode " <<  f << " f " << t[v].len << std::endl;
          while (j != f) {
            answer.insert(str.substr(t[v].endpos + 1 - f + j, f - j));
            answer.insert(str.substr(t[v].endpos + 1 - f + j, f - j) + c);
            j++;
          }
          v = t[v].to[GetIndex(c)];
          f = f + 1;
          fromsuf = false;
        }
      }
      if (v == -1) {
        v = 0;
        f = 0;
        fromsuf = false;
      }
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::string secondstr;
  std::string stroka;
  std::cin >> stroka;
  std::cin >> secondstr;
  int64_t k = 0;
  std::cin >> k;
  SuffAuto suff1(stroka, k);
  // suff1.Print();
  suff1.Answer(secondstr, stroka);
  if (static_cast<int64_t>(suff1.answer.size()) < k) {
    std::cout << -1 << std::endl;
  } else {
    auto it = std::next(suff1.answer.begin(), k - 1);
    std::cout << *it << std::endl;
  }
}