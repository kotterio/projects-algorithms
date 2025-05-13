#ifndef OPTIONAL_H
#define OPTIONAL_H
#include <stdexcept>
#include <array>

class BadOptionalAccess : public std::exception {
  public: 
    char const* what() const noexcept override {
      return "BadOptionalAccess";
    }
};

template <class T>
struct alignas(8) Optional {
  std::array<char, sizeof(T)> data;
  bool has_value = false;

  Optional() = default;

  Optional(const Optional& other) {
    if (other.has_value) {
      new (&data) T(other.Value());
      has_value = true;
    } else {}
  }

  Optional(Optional&& other) {
    if (!other.has_value) {
    } else {
    new (&data) T(std::move(reinterpret_cast<T&>(other.data)));
    has_value = true;
    }
  }

  Optional(const T& other)  {//NOLINT
    has_value = true;
    new (&data) T(other);
  }
  Optional(T&& other) {//NOLINT{
    has_value = true;
    new (&data) T(std::move(other));
  }
  ~Optional() {
    if (has_value) {
      (reinterpret_cast<T*>(&data))->~T();
      has_value = false;
    }
  }
  Optional& operator=(const Optional& other) {
    if (this == &other) {
      return *this;
    }
    if (!other.has_value) {
      if (has_value) {
        (reinterpret_cast<T*>(&data))->~T(); 
        has_value = false;
      }
      return *this;
    }
    if (!has_value) {
      new (&data) T(other.Value());
      has_value = true;
    } else {
      Value() = other.Value();
    }
    return *this;
  }
  Optional& operator=(Optional&& other) {
    if (this == &other) {
      return *this;
    }
    if (!other.has_value) {
      if (has_value) {
        (reinterpret_cast<T*>(&data))->~T(); 
        has_value = false;
      }
      return *this;
    }
    if (!has_value) {
      new (&data) T(std::move(other.Value()));
      has_value = true;
    } else {
      Value() = std::move(other.Value());
    }
    return *this; // to
  }
  Optional& operator=(const T& other) {
    if (has_value) {
      Value() = other;
      return *this;
    }
    new (&data) T(other);
    has_value = true;
    return *this;
  }
  Optional& operator=(T&& other) {
    if (has_value) {
      Value() = std::move(other);
      return *this;
    }
    new (&data) T(other);
    has_value = true;
    return *this;
  }
  bool HasValue() const {
    return has_value;
  }
  explicit operator bool() const noexcept {
    return has_value;
  }
  const T& Value() const {
    if (!has_value) {
      throw BadOptionalAccess();
    }
    return reinterpret_cast<const T&>(data);
  }
  T& Value() {
    if (!has_value) {
      throw BadOptionalAccess();
    }
    return reinterpret_cast<T&>(data);
  }
  T& operator*() {
    return *reinterpret_cast<T*>(&data);
  }
  const T& operator*() const {
    return *reinterpret_cast<const T*>(&data);
  }
  template <class... Args>
  T& Emplace(Args&&... args) {
    if (has_value) {
      Value().~T();
    }
    new (&data) T(std::forward<Args>(args)...);
    has_value = true;
    return Value();
  }
  void Reset() {
    if (has_value) {
      Value().~T();
      has_value = false;
    }
  }
  void Swap(Optional& other) {
    if (has_value && other.has_value) {
      std::swap(Value(), other.Value());
    } else if (!has_value && !other.has_value) {
    } else if (has_value && !other.has_value) {
      other = std::move(Value());
      Value().~T();
      has_value = false;
    } else {
      *this = std::move(other.Value());
      other.Value().~T();
      other.has_value = false;
    }
  }
};
#endif