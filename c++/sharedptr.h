
#ifndef SHAREDPTR_H
#define SHAREDPTR_H
#include <cstddef>
#include <stdexcept>
class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};
template<class T>
class SharedPtr{
    T* data_;
    size_t* count_;
    public:
        SharedPtr(): data_(nullptr), count_(nullptr){}
        SharedPtr(T* origptr): data_(origptr) // NOLINT
        { 
            if(data_ == nullptr){
                count_ = nullptr;
            }
            else{
                count_ = new size_t;
                *count_ = 1;
            }
        }
        SharedPtr& operator=(T* rowptr ) {
            if(data_ != nullptr){
                if(*count_ == 1){
                    delete data_;
                    delete count_;
                }
                else{
                    *count_ -=1;
                }
            }
            data_ = rowptr;
            if (data_ == nullptr) {
                count_ = nullptr;
                //count_ = new size_t{0};
            }
            else {
                count_ = new size_t{1};
            }
            return *this;
        }
        SharedPtr(const SharedPtr& other): data_(other.data_), count_(other.count_){
            if(count_ != nullptr){
                *count_ += 1;
            }
        }
        SharedPtr& operator=(const SharedPtr& other){
            if(this == &other){
                return *this;
            }
            if(data_ != nullptr){
                if(*count_ == 1){
                    delete data_;
                    delete count_;
                }
                else{
                    *count_ -=1;
                }
            }
            data_ = other.data_;
            count_ = other.count_;
            if(count_ != nullptr){
                *count_ += 1;
            }
            return *this;
        }
        SharedPtr(SharedPtr&& other) noexcept: data_(other.data_), count_(other.count_){
            other.data_ = nullptr;
            other.count_ = nullptr;
        }
        SharedPtr& operator=(SharedPtr&& other) noexcept {
            if(this == &other){
                return *this;
            }
            if(data_ != nullptr){
                if(*count_ == 1){
                    delete data_;
                    delete count_;
                }
                else{
                    *count_ -=1;
                }
            }
            data_ = other.data_;
            count_ = other.count_;
            other.data_ = nullptr;
            other.count_ = nullptr;
            return *this;
        }
        ~SharedPtr(){
            if(count_ != nullptr){
                if(*count_ == 1){
                    delete data_;
                    delete count_;
                }
                else{
                    *count_ -=1;
                }
            }
        }
        void Reset(T* ptr = nullptr){
            if(data_ != nullptr){
                if(*count_ == 1){
                    delete data_;
                    delete count_;
                }
                else{
                    *count_ -=1;
                }
            }
            data_ = ptr;
            count_ = nullptr;
            if(data_ != nullptr){
                count_ = new size_t;
                *count_ = 1;
            }
        }
        void Swap(SharedPtr& other){
            T* tmp = data_;
            data_ = other.data_;
            other.data_ = tmp;
            size_t* tcount = count_;
            count_ = other.count_;
            other.count_ = tcount;
        }
        T* Get() const{
            return data_;
        }
        size_t UseCount() const{
            if(count_ == nullptr){
                return 0;
            }
            return *count_;
        }
        T& operator*(){
            return *data_;
        }
        T& operator*() const{
            return *data_;
        }
        T* operator->() const{
            return data_;
        }
        explicit operator bool() const{
            return data_!= nullptr;
        }
};

#endif
// YOUR CODE...
