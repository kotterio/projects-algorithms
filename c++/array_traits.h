#ifndef ARRAY_TRAITS_H
#define ARRAY_TRAITS_H
#include <cstdint>
#include <iostream>
#include <iterator>
#include <type_traits>

template <class T>
struct Isarray : std::false_type {};

template <class T>
struct Isarray<T[]> : std::true_type {};

template <class T, std::size_t N>
struct Isarray<T[N]> : std::true_type {};

template <class T>
constexpr bool kIsArrayV = Isarray<T>::value;

template <class T>
struct Arraysize : std::integral_constant<size_t, 1> {};

template <class T, std::size_t N>
struct Arraysize<T[N]> : std::integral_constant<size_t, N> {};

template <class T, std::size_t N>
struct Arraysize<const T[N]> : std::integral_constant<size_t, N> {};

template <class T>
struct Arraysize<T[]> : std::integral_constant<size_t, 0> {};

template <class T>
struct Arraysize<const T[]> : std::integral_constant<size_t, 0> {};

template <class T>
constexpr size_t kSizeV = Arraysize<T>::value;

template <class T>
struct Arrayrank : std::integral_constant<size_t, 0> {};

template <class T>
struct Arrayrank<T[]> : std::integral_constant<size_t, Arrayrank<T>::value + 1> {};

template <class T>
struct Arrayrank<const T[]> : std::integral_constant<size_t, Arrayrank<T>::value + 1> {};

template <class T, std::size_t N>
struct Arrayrank<const T[N]> : std::integral_constant<size_t, Arrayrank<T>::value + 1> {};

template <class T, std::size_t N>
struct Arrayrank<T[N]> : std::integral_constant<size_t, Arrayrank<T>::value + 1> {};

template <class T>
constexpr size_t kRankV = Arrayrank<T>::value;

template <class T>
struct ArrayFullsize : std::integral_constant<size_t, 1> {};

template <class T, std::size_t N>
struct ArrayFullsize<T[N]> : std::integral_constant<size_t, ArrayFullsize<T>::value * N> {};

template <class T, std::size_t N>
struct ArrayFullsize<const T[N]> : std::integral_constant<size_t, ArrayFullsize<T>::value * N> {};

template <class T>
struct ArrayFullsize<T[]> : std::integral_constant<size_t, 0> {};

template <class T>
struct ArrayFullsize<const T[]> : std::integral_constant<size_t, 0> {};

template <class T>
constexpr size_t kTotalSizeV = ArrayFullsize<T>::value;

template <class T>
struct Type {
  using T2 = T;
};

template <class T>
struct UpcellType : Type<T> {};

template <class T, std::size_t N>
struct UpcellType<T[N]> : Type<T> {};

template <class T>
struct UpcellType<T[]> : Type<T> {};

template <class T>
using RemoveArrayT = typename UpcellType<T>::T2;

template <class T>
struct DowncellType : Type<T> {};

template <class T, std::size_t N>
struct DowncellType<T[N]> : DowncellType<T> {};

template <class T>
struct DowncellType<T[]> : DowncellType<T> {};

template <class T>
using RemoveAllArraysT = typename DowncellType<T>::T2;

#endif