#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <utility>
#include <random>

struct Dec {
  std::vector<int64_t> deg;
  std::vector<int64_t> hs;
  std::unordered_set<int64_t> des;
};

void Preprocess(std::string& st, int64_t p, int64_t m, Dec& dec) {
  for (int64_t i = 0; i < static_cast<int64_t>(st.length()); i++) {
    dec.hs[i + 1] = (dec.hs[i] * p + st[i]) % m;
    dec.deg[i + 1] = (dec.deg[i] * p) % m;
  }
}

int64_t HashS(Dec& dec, int64_t l, int64_t r) {
  int64_t m = INT64_MAX;
  return (dec.hs[r] - dec.hs[l] * dec.deg[r - l]) % m;
}

void Desicion(std::string& st, Dec& dec) {
  // std::cout << st;
  auto n = static_cast<int64_t>(st.length());
  int64_t min = 2;
  int64_t max = INT64_MAX - 1;
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int64_t> dist(min, max - 1);
  int64_t p = dist(generator);
  Preprocess(st, p, INT64_MAX, dec);
  for (int64_t start = 0; start < n; start++) {
    for (int64_t length = 1; length <= n - start; ++length) {
      dec.des.insert(HashS(dec, start, start + length));
    }
  }
}

int main() {
  std::string s;
  std::cin >> s;
  Dec d;
  d.hs = std::vector<int64_t>(s.length() + 1, 0);
  d.deg = std::vector<int64_t>(s.length() + 1, 1);
  Desicion(s, d);
  std::cout << d.des.size() << std::endl;
}