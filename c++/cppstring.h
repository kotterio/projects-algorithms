#ifndef CPPSTRING_H
#define CPPSTRING_H


#include <stdexcept>
#include <iostream>



class String{
    char* data_;
    size_t size_;
    size_t capacity_;
    void Myrealloc(size_t);
    size_t Mystrlen(const char*);
    public:
        String();
        String(const char*); // NOLINT
        String(size_t, char);
        String(const char*, size_t);
        String(const String&);
        String& operator=(const String&);
        ~String();
        char operator[](size_t) const;
        char& operator[](size_t);
        char* CStr() ;
        char* Data() ;
        const char* CStr() const;
        const char* Data() const;
        char& At(size_t) const;
        char& At(size_t);
        char& Front();
        char& Back();
        const char& Front() const;
        const char& Back() const;
        bool Empty() const;
        size_t Size() const;
        size_t Length() const;
        size_t Capacity() const;
        void Clear();
        void Swap(String&);
        void PopBack();
        void PushBack(char);
        String& operator+=(const String&);
        void Resize(size_t ,char);
        void Reserve(size_t);
        void ShrinkToFit();
        friend String operator+(const String&, const String&);
        friend bool operator<(const String&, const String&);
        friend bool operator>(const String&, const String&);
        friend bool operator<=(const String&, const String&);
        friend bool operator>=(const String&, const String&);
        friend bool operator==(const String&, const String&);
        friend bool operator!=(const String&, const String&);
        friend std::ostream& operator<<(std::ostream&, const String&);
};

class StringOutOfRange : public std::out_of_range {
    public:
    StringOutOfRange() : std::out_of_range("StringOutOfRange") {}
};

#endif

