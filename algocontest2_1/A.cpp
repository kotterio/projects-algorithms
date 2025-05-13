#include <iostream>
#include <vector>
#include <string>
#include <set>


size_t Max(std::string& text) {
  size_t n = text.length();
  size_t max = 0;
  std::string string;
  std::string st;
  for (size_t i = 0; i < n; i++) {
    st = text.substr(i, n - i);
    size_t k = 1;
    size_t med = 1;
    std::string t = text.substr(i, 1);
    string = t + "#" + st;
    std::vector<size_t> prefirst = ZFunction(string, 1);
    std::cout << text[i] << std::endl;
    for (size_t elem = 0; elem < prefirst.size(); elem++) {
      std::cout << prefirst[elem] << std::endl;
      while (prefirst[elem] + 1 < st.size() && prefirst[elem] == prefirst[elem + 1]) {
        k++;
        elem++;
      }
      if (med < k) {
        med = k;
      }
      k = 0;
    }
    for (size_t j = i + 1; j < n; j++) {
      med = 1;
      k = 1;
      size_t size = j - i + 1;
      std::vector<size_t> presecond;
      size_t preindex = prefirst[0];
      presecond.reserve(prefirst.size());
      for (size_t elem = 0; elem < prefirst.size(); elem++) {
        if(prefirst[elem] + 1 < st.length() && st[prefirst[elem] + 1] == st[j]) {
          presecond.push_back(prefirst[elem]);
          std::cout << " here" << prefirst[elem] << std::endl;
          if (preindex + size == prefirst[elem]) {
            k++;
            preindex = prefirst[elem];
            std::cout << "еге" << preindex << std::endl;
          } else {
            if (med < k) {
              med = k;
              k = 1;
              preindex = prefirst[elem];
            }
          }
        }
      }
      for (auto elem : presecond) {
        std::cout << "la" << elem;
      }
      if (med < k) {
        med = k;
      }
      if (med > max) {
        max = med;
      }
      prefirst = presecond;
    }
  }
  return max;
}

int main() {
  std::string text;
  std::cin >> text;
  std::cout << Max(text);
}
