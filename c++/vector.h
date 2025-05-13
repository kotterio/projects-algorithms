#ifndef VECTOR_H
#define VECTOR_H
#define VECTOR_MEMORY_IMPLEMENTED
#include <iostream>
#include <iterator>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <cstddef>
#include <type_traits>

template<typename T> class Vector {
  T* data_;
  size_t size_;
  size_t capacity_;
  T* Allocate(const size_t& newcap) noexcept {
    T* buf = reinterpret_cast<T*>(operator new(newcap * sizeof(T)));
    return buf;
  }
  void Realloc(size_t newsize, T& x) {
    if(size_ == newsize) {
      return;
    }
    if (newsize > size_) {
      if (newsize <= capacity_) {
        size_t k = size_;
        for (; k < newsize; ++k) {
          new (data_ + k) T(std::move(x));
        }
        size_ = newsize;
      } else {
        T* buffer = Allocate(newsize);
        for (size_t i = 0; i < size_; i++) {
          //buffer[i] = data_[i];
          new (buffer + i) T(std::move(data_[i]));
        }
        size_t k = size_;
        size_t oldsize = size_;
        for (; k < newsize; k++) {
          try {
            new (buffer + k) T(std::move(x));
            size_++;
          }
          catch(...) {
            for(size_t i = 0; i < oldsize; i++) {
              data_[i].~T();
            }
            operator delete (data_);
            data_ = buffer;
            capacity_ = newsize;
            throw;
          }
        }
        for(size_t i = 0; i < oldsize; i++) {
          data_[i].~T();
        }
        operator delete (data_);
        data_ = buffer;
        size_ = newsize;
        capacity_ = newsize;
      }
    } else {
      for(size_t i = newsize; i < size_; i++) {
        data_[i].~T();
      }
      size_ = newsize;
    }
  }
  void Realloc(size_t newsize) {
   if(size_ == newsize) {
      return;
    } 
    if (newsize > size_) {
      if (newsize <= capacity_) {
        size_t k = size_;
        for (; k < newsize; ++k) {
          new (data_ + k) T();
        }
        size_ = newsize;
      } else {
        T* buffer = Allocate(newsize);
        for (size_t i = 0; i < size_; i++) {
          //buffer[i] = data_[i];
          new (buffer + i) T(std::move(data_[i]));
        }
        size_t k = size_;
        size_t oldsize = size_;
        for (; k < newsize; k++) {
          try {
            new (buffer + k) T();
            size_++;
          }
          catch(...) {
            for(size_t i = 0; i < size_; i++) {
              buffer[i].~T();
            }
            operator delete (buffer);
            size_ = oldsize;
            throw;
          }
        }
        for(size_t i = 0; i < oldsize; i++) {
          data_[i].~T();
        }
        operator delete (data_);
        data_ = buffer;
        size_ = newsize;
        capacity_ = newsize;
      }
    } else {
      for(size_t i = newsize; i < size_; i++) {
        data_[i].~T();
      }
      size_ = newsize;
    }
  }
  void Realloc2(size_t newsize) {
   if(size_ == newsize) {
      return;
    } 
    if (newsize > size_) {
      if (newsize <= capacity_) {
        size_t k = size_;
        for (; k < newsize; ++k) {
          new (data_ + k) T();
        }
      } else {
        T* buffer = Allocate(newsize);
        for (size_t i = 0; i < size_; i++) {
          //buffer[i] = data_[i];
          new (buffer + i) T(std::move(data_[i]));
        }
        size_t oldsize = size_;
        for(size_t i = 0; i < oldsize; i++) {
          data_[i].~T();
        }
        operator delete (data_);
        data_ = buffer;
        capacity_ = newsize;
      }
    } else {
      for(size_t i = newsize; i < size_; i++) {
        data_[i].~T();
      }
    }
  }
  
  public: 
    using Pointer = T*;
    using ValueType = T;
    using ConstPointer = T*;
    using Reference = T&;
    using ConstReference = const T&;
    using SizeType = size_t;
    using Iterator = T*;
    using ConstIterator = const T*;
    using ReverseIterator = std::reverse_iterator<T*>;
    using ConstReverseIterator = std::reverse_iterator<const T*>;

    Iterator begin() {  // NOLINT
      return data_;
    }

    Iterator end() {  // NOLINT
      return (data_ + size_);
    }

    ConstIterator begin() const {  // NOLINT
      return data_;
    }

    ConstIterator end() const {  // NOLINT
      return (data_ + size_);
    }

    ConstIterator cbegin() const {  // NOLINT
      return data_;
    }

    ConstIterator cend() const {  // NOLINT
      return (data_ + size_);
    }

    ReverseIterator rbegin() {  // NOLINT
      return std::reverse_iterator(data_ + size_);
    }

    ReverseIterator rend() {  // NOLINT
      return std::reverse_iterator(data_);
    }

    ConstReverseIterator rbegin() const {  // NOLINT
      return std::reverse_iterator(data_ + size_);
    }

    ConstReverseIterator rend() const {  // NOLINT
      return std::reverse_iterator(data_);
    }

    ConstReverseIterator crbegin() const {  // NOLINT
      return std::reverse_iterator(data_ + size_);
    }

    ConstReverseIterator crend() const {  // NOLINT
      return std::reverse_iterator(data_);
    }
    /*
    struct Iterator{
      using iterator_category = std::random_access_iterator_tag;
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = T*;
      using reference = T&;
      Iterator(pointer p) : p_(p) {}
      reference operator*() const {
        return *p_;
      }
      pointer operator->() {
        return p_;
      }
      Iterator& operator++() {
        p_++;
        return *this;
      }
      Iterator operator++(int) {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
      }
      Iterator& operator--() {
        p_--;
        return *this;
      }

      Iterator operator--(int) {
        Iterator tmp = *this;
        --(*this);
        return tmp;
      }

      Iterator operator+(long int dp) {
        return Iterator(p_ + dp);
      }

      Iterator operator-(long int dp) {
        return Iterator(p_ - dp);
      }

      friend bool operator==(const Iterator& a, const Iterator& b) {
        return a.p_ == b.p_; 
      }
      friend bool operator!=(const Iterator& a, const Iterator& b) {
        return a.p_ != b.p_; 
      }
      private:
        pointer p_;
    };
    struct ConstIterator{
      using iterator_category = std::random_access_iterator_tag;
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = const T*;
      using reference = const T&;
      ConstIterator(pointer p) : p_(p) {}
      reference operator*() const {
        return *p_;
      }
      pointer operator->() {
        return p_;
      }
      ConstIterator operator-(long int dp) {
        return ConstIterator(p_ - dp);
      }
      ConstIterator& operator++() {
        p_++;
        return *this;
      }
      ConstIterator& operator--() {
        p_--;
        return *this;
      }

      ConstIterator operator--(int) {
        ConstIterator tmp = *this;
        --(*this);
        return tmp;
      }
      ConstIterator operator+(long int dp) {
        return ConstIterator(p_ + dp);
      }
      ConstIterator operator++(int) {
        ConstIterator tmp = *this;
        ++(*this);
        return tmp;
      }
      friend bool operator==(const ConstIterator& a, const ConstIterator& b) {
        return a.p_ == b.p_; 
      }
      friend bool operator!=(const ConstIterator& a, const ConstIterator& b) {
        return a.p_ != b.p_; 
      }
      private:
        pointer p_;
    };*/
    /*
    struct ReverseIterator{
      using iterator_category = std::random_access_iterator_tag;
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = T*;
      using reference = T&;
      ReverseIterator(pointer p) : p_(p) {}
      reference operator*() const {
        return *p_;
      }
      pointer operator->() {
        return p_;
      }
      ReverseIterator& operator++() {
        p_--;
        return *this;
      }
      ReverseIterator operator+(ptrdiff_t dp) {
        return ReverseIterator(p_ - dp);
      }
      ReverseIterator operator++(int) {
        ReverseIterator tmp = *this;
        --(*this);
        return tmp;
      }
      friend bool operator==(const ReverseIterator& a, const ReverseIterator& b) {
        return a.p_ == b.p_; 
      }
      friend bool operator!=(const ReverseIterator& a, const ReverseIterator& b) {
        return a.p_ != b.p_; 
      }
      private:
        pointer p_;
    };
    struct ConstReverseIterator{
      using iterator_category = std::random_access_iterator_tag;
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = const T*;
      using reference = const T&;
      ConstReverseIterator(pointer p) : p_(p) {}
      reference operator*() const {
        return *p_;
      }
      pointer operator->() {
        return p_;
      }
      ConstReverseIterator& operator++() {
        p_--;
        return *this;
      }
      ConstReverseIterator operator+(ptrdiff_t dp) {
        return ConstReverseIterator(p_ - dp);
      }
      ConstReverseIterator operator++(int) {
        ReverseIterator tmp = *this;
        --(*this);
        return tmp;
      }
      friend bool operator==(const ConstReverseIterator& a, const ConstReverseIterator& b) {
        return a.p_ == b.p_; 
      }
      friend bool operator!=(const ConstReverseIterator& a, const ConstReverseIterator& b) {
        return a.p_ != b.p_; 
      }
      private:
        pointer p_;
    };*/

    /*
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    Iterator begin() {
      return Iterator(data_);
    }
    ConstIterator begin() const {
      return ConstIterator(data_);
    }
    Iterator end() {
      return Iterator(data_ + size_);
    }
    ConstIterator end() const {
      return ConstIterator(data_ + size_);
    }
    ConstIterator cbegin() {
      return ConstIterator(data_);
    }
    ConstIterator cbegin() const {
      return ConstIterator(data_);
    }
    ConstIterator cend() {
      return ConstIterator(data_ + size_);
    }
    ConstIterator cend() const {
      return ConstIterator(data_ + size_);
    }
    ReverseIterator rbegin() {
      //return ReverseIterator(data_ + size_ - 1);
      return std::make_reverse_iterator(Iterator(data_ + size_));
    }
    ConstReverseIterator rbegin() const {
      //return ConstReverseIterator(data_ + size_ - 1);
      return std::reverse_iterator(ConstIterator(data_ + size_));
    }
    ReverseIterator rend() {
      //return ReverseIterator(data_ - 1);
      return std::make_reverse_iterator(Iterator(data_));
    }
    ConstReverseIterator rend() const{
      //return ConstReverseIterator(data_ - 1);
      return std::reverse_iterator(ConstIterator(data_ - 1));
    }
    ConstReverseIterator crbegin() {
      //return ConstReverseIterator(data_ + size_ - 1);
      return std::reverse_iterator(ConstIterator(data_ + size_ - 1));
    }
    ConstReverseIterator crbegin() const {
      //return ConstReverseIterator(data_ + size_ - 1);
      return std::reverse_iterator(ConstIterator(data_ + size_ - 1));
    }
    ConstReverseIterator crend() {
      //return ConstReverseIterator(data_ - 1);
      return std::reverse_iterator(ConstIterator(data_ - 1));
    }
    ConstReverseIterator crend() const {
      //return ConstReverseIterator(data_ - 1);
      return std::reverse_iterator(ConstIterator(data_ - 1));
    }*/

    Vector(): data_(nullptr), size_(0), capacity_(0) {}
    explicit Vector(size_t count) {
      size_t i = 0;
      try {
        size_ = count;
        capacity_ = count;
        data_ = Allocate(count);
        for (; i < count; i++) {
          new (data_ + i) T();
        }
      } catch(...) {
        for (size_t j = 0; j < i; j++) {
          data_[j].~T();
        }
        operator delete (data_);
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
        throw;
      }
    }
    Vector(size_t count, const T& value) {
      size_t i = 0;
      try {
        size_ = count;
        capacity_ = count;
        if (count == 0) {
          data_ = nullptr;
          return;
        }
        data_ = Allocate(count);
        for (; i < count; i++) {
          new (data_ + i) T(value);
        }
      } catch(...) {
        for (size_t j = 0; j < i; j++) {
          data_[j].~T();
        }
        operator delete (data_);
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
        throw;
      }
    }
    template <class Iterator, class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
    Vector(Iterator first, Iterator last) {
      size_t k = 0;
      try {
        if (first != last) {
          size_ = std::distance(first, last);
          capacity_ = size_;
          data_ = Allocate(size_);
          while (k < size_) {
          //data_[k] = *first;
            new (data_ + k) T(*first);
            first++;
            k++;
          }
        } else {
          size_ = 0;
          capacity_ = 0;
          data_ = nullptr;
          return;
        }
      } catch(...) {
        for (size_t j = 0; j < k; j++) {
          data_[j].~T();
        }
        operator delete (data_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
    Vector(std::initializer_list<T> list) {
      size_t i = 0;
      try {
        if (list.size() == 0) {
          size_ = 0;
          capacity_ = 0;
          data_ = nullptr;
        } else {
          size_ = capacity_ = std::distance(list.begin(), list.end());
          data_ = Allocate(size_);
          const T* cur = list.begin();
          while(cur != list.end()) {
          //data_[k] = *cur;
            new (data_ + i) T(*cur);
            i++;
            cur++;
          }
        }
      } catch(...) {
        for (size_t j = 0; j <= i; j++) {
          data_[j].~T();
        }
        operator delete (data_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
    Vector(const Vector& other) {
      size_t i = 0;
      try {
        if (other.size_ == 0) {
          size_ = 0;
          capacity_ = 0;
          data_ = nullptr;
        } else {
          size_ = other.size_;
          capacity_ = size_;
          data_ = Allocate(size_);
          for (; i < size_; i++) {
          //data_[i] = other.data_[i];
            new (data_ + i) T(other.data_[i]);
          }
        }
      } catch(...) {
        for (size_t j = 0; j <= i; j++) {
          data_[j].~T();
        }
        operator delete (data_);
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
        throw;
      }
    }
    Vector(Vector&& other) {
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    Vector& operator=(const Vector& other) {
      if (this == &other) {
        return *this;
      }
      if (other.size_ == 0) {
        for (size_t j = 0; j < size_; j++) {
          data_[j].~T();
        }
        operator delete (data_);
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
      } else {
        for (size_t i = 0; i < size_; i++) {
          data_[i].~T();
        }
        size_ = other.size_;
        operator delete (data_);
        data_ = Allocate(size_);
        capacity_ = size_;
        size_t i = 0;
        try {
          for (; i < size_; i++) {
            new (data_ + i) T(other.data_[i]);
            // data_[i] = other.data_[i];
          }
        } catch(...) {
          for (size_t j = 0; j < i; j++) {
            data_[j].~T();
          }
          operator delete(data_);
          size_ = capacity_ = 0;
          data_ = nullptr;
          throw;
        }
      }
      return *this;
    }
    Vector& operator=(Vector&& other) {
      if(this == &other) {
        return *this;
      }
      for (size_t i = 0; i < size_; i++) {
          data_[i].~T();
      }
      operator delete (data_);
      size_ = other.size_;
      capacity_ = other.capacity_;
      data_ = other.data_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
      return *this;
    }
    ~Vector() noexcept{
      for (size_t i = 0; i < size_; i++) {
        data_[i].~T();
      }
      operator delete(data_); //to do
    }
    size_t Size() const {
      return size_;
    }
    size_t Capacity() const {
      return capacity_;
    }
    bool Empty() const {
      return (size_ == 0);
    }
    T& operator[](size_t ind) {
      return *(data_ + ind);
    }
    const T& operator[](size_t ind) const {
      return *(data_ + ind);
    }
    T& At(size_t ind) {
      if(ind >= size_) {
        throw std::out_of_range("out_of_range");
      }
      return *(data_ + ind);
    }
    const T& At(size_t ind) const {
      if(ind >= size_) {
        throw std::out_of_range("out_of_range");
      }
      return *(data_ + ind);
    }
    T& Front() {
      return *(data_);
    }
    const T& Front() const {
      return *(data_);
    }
    T* Data() const {
      return data_;
    }
    T& Back() {
      return *(data_ + size_ -1);
    }
    const T& Back() const{
      return *(data_ + size_ - 1);
    }
    void Swap(Vector& other) {
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      std::swap(data_, other.data_);
    }
    void Resize(size_t newsize) {
      Realloc(newsize);
    }
    void Resize(size_t newsize, T value) {
      Realloc(newsize, value);
    }
    void Reserve(size_t new_cap) {
      if(new_cap <= capacity_) {
        return;
      }
      T* buffer =Allocate(new_cap);
      for(size_t i = 0; i < size_; i++) {
        //buffer[i] = data_[i];
        new (buffer + i) T(std::move(data_[i]));
      }
      for (size_t i = 0; i < size_; i++) {
          data_[i].~T();
      }
      operator delete (data_); // to do
      data_ = buffer;
      capacity_ = new_cap;
    }
    void ShrinkToFit() {
      if (size_ == capacity_) {
        return;
      }
      //T* buffer = new T[size_];
      if (size_ > 0) {
        T* buffer = Allocate(size_);
        for(size_t i = 0; i < size_; i++) {
        //buffer[i] = data_[i];
          new (buffer + i) T(std::move(data_[i]));
        }
        for (size_t i = 0; i < size_; i++) {
          data_[i].~T();
        }
        operator delete (data_); // to do
        data_ = buffer;
        capacity_ = size_;
      } else {
        operator delete (data_);
        data_ = nullptr;
        capacity_ = 0;
      }
    }
    void Clear() {
      for (size_t i = 0; i < size_; i++) {
          data_[i].~T();
      }
      size_ = 0;
    }
    void PushBack(const T& val) {
      if (size_ == 0 && capacity_ == 0) {
        Reserve(1);
        new (data_ + size_) T(val);
        size_++;
        return;
      } if (size_ == capacity_) {
        const T* q = nullptr;
        try {
          q = new T(val);
          Reserve(2 * capacity_);
          new (data_ + size_) T(val);
          size_++;
          delete q;
        }
        catch (...) {
          delete q;
          throw;
        }
        return;
      } 
      new (data_ + size_) T(val);
      size_++;
      //data_[size_] = val;
    }
    template<class ...Args>
    void EmplaceBack(Args&&... args) {
      if (capacity_ == 0) {
        Reserve(1);
      } else if (size_ == capacity_) {
        Reserve(size_ * 2);
      }
      new (data_ + size_++) T(std::forward<Args>(args)...);

    }



    void PushBack(T&& val) {
      if (size_ == 0) {
        Reserve(1);
      }
      else if (size_ == capacity_) {
        Reserve(2 * capacity_);
      }
      //data_[size_] = val;
      new (data_ + size_) T(std::move(val));
      size_++;
    }
    void PopBack() {
      if (size_ > 0) {
        data_[size_-1].~T();
        size_--;
      }
    }
    bool operator<(const Vector& other) const {
      size_t k = 0;
      while (k < size_ && k < other.size_) {
        if (data_[k] < other.data_[k]) {
          return true;
        }
        if (data_[k] > other.data_[k]) {
          return false;
        }
        k++;
      }
      return size_ < other.size_;
    }
    bool operator>(const Vector& other) const {
      size_t k = 0;
      while (k < size_ && k < other.size_) {
        if (data_[k] > other.data_[k]) {
          return true;
        }
        if (data_[k] < other.data_[k]) {
          return false;
        }
        k++;
      }
      return size_ > other.size_;
    }
    bool operator>=(const Vector& other) const {
      return !operator<(other);
    }
    bool operator<=(const Vector& other) const {
      return !operator>(other);
    }
    bool operator==(const Vector& other) const {
      return !(*this < other) && !(*this > other);
    }
    bool operator!=(const Vector& other) const {
      return (*this < other || *this > other);
    }
};

#endif