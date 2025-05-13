#ifndef ITEROPS_H
#define ITEROPS_H
#include <iterator>
#include <type_traits>
#include <iostream>

template <class Myiterator, class Mydist>
constexpr void Advance(Myiterator& it, Mydist delta) {
  using Category = typename std::iterator_traits<Myiterator>::iterator_category;
  auto step = static_cast<typename std::iterator_traits<Myiterator>::difference_type>(delta);
  if constexpr (std::is_base_of_v<std::random_access_iterator_tag, Category>) {
    it += step;
  } else {
    while (step > 0) {
      ++it;
      --step;
    }
    if constexpr (std::is_base_of_v<std::bidirectional_iterator_tag, Category>) {
      while (step < 0) {
        ++step;
        --it;
      }
    }
  }
}

template <class Myiterator>
constexpr Myiterator Next(Myiterator it, typename std::iterator_traits<Myiterator>::difference_type step = 1) {
  Advance(it, step);
  return it;
}

template <class Myiterator>
constexpr Myiterator Prev(Myiterator it, typename std::iterator_traits<Myiterator>::difference_type step = 1) {
  Advance(it, -step);
  return it;
}

template <class Myiterator>
constexpr typename std::iterator_traits<Myiterator>::difference_type Distance(Myiterator it1, Myiterator it2) {
  using Category = typename std::iterator_traits<Myiterator>::iterator_category;
  if constexpr (std::is_base_of_v<std::random_access_iterator_tag, Category>) {
    return it2 - it1;
  } else {
    typename std::iterator_traits<Myiterator>::difference_type res = 0;
    while (it1 != it2) {
      ++it1;
      ++res;
    }
    return res;
  }
}
#endif