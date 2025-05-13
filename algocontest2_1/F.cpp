#include <vector>
#include <iostream>
#include <algorithm>

size_t ZFunction(const std::string& st) {
  std::vector<size_t> z(st.size(), 0);
  size_t index = 0;
  size_t left = 0;
  size_t right = 0;
  for (size_t i = 1; i < st.size(); i++) {
    if (i < right) {
      z[i] = std::min(z[i - left], right - i);
    }
    while (i + z[i] < st.size() && st[z[i]] == st[i + z[i]]) {
      ++z[i];
    }
    if (index < z[i]) {
      index = z[i];
    }
    if (right < i + z[i]) {
      left = i;
      right = i + z[i];
    }
  }
  return index;
}

int main() {
  std::string text;
  std::cin >> text;
  std::string st;
  size_t count = 0;
  for (size_t i = 0; i < text.length(); i++) {
    st = text.substr(0, i + 1);
    std::reverse(st.begin(), st.end());
    count += st.size() - ZFunction(st);
  }
  std::cout << count;
}