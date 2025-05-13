#include "cppstring.h"
#include <stdexcept>

void String::Myrealloc(size_t nb){
	char* newdata = new char[nb];
	if(size_ > nb){
		size_ = nb;
	}
	for(size_t k = 0; k < size_; ++k){
		newdata[k] = data_[k];
	}
	capacity_ = nb;
	delete[] data_;
	data_ = newdata;
}
size_t String::Mystrlen(const char* s){
	if(s==nullptr){
		return 0;
	}
	size_t k = 0;	
	while(s[k] != 0){
		++k;
	}
	return k;		
}


String::String(){
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}
String::String(const char* str){
	if(str==nullptr){
		data_ = nullptr;
		size_ = 0;
		capacity_ = 0;
	}
	else{
    	size_t k = Mystrlen(str);    	
    	capacity_ = k;
    	size_ = k;
    	data_ = new char[capacity_];    	
    	for(size_t j=0; j < capacity_; ++j){
        	data_[j] = str[j];
    	}
    	size_ = capacity_;
    }
}
String::String(size_t n, char x){
    capacity_ = n;
    size_ = n;
    if(n==0){
    	data_ = nullptr;    	
    }
    else{
    	data_ = new char[capacity_];        	
    	for(size_t i =0; i < n; ++i){
        	data_[i] = x;
    	}
    }
}
String::String(const char* str, size_t n){
	if(n == 0 || str==nullptr || str[0]==0){
		data_ = nullptr;
		size_ = 0;
		capacity_ = 0;	
	}
	else{
    	size_t k = 0;
    	while(k<n && str[k] != 0 ){
        	++k;
    	}
    	capacity_ = k;
    	size_ = k;
    	data_ = new char[capacity_];
    	for(size_t i = 0; i< size_; ++i){
        	data_[i] = str[i];
    	}    	
    }
}
String::String(const String& orig){
    size_ = orig.size_;
    capacity_ = orig.capacity_;
    if(size_==0){
    	data_ = nullptr;
    }
    else{
    	data_ = new char[capacity_];
    	for(size_t i = 0; i<size_; ++i){
        	data_[i] = orig.data_[i];
    	}    
    }
}
String& String::operator=(const String& orig){
    if(this == &orig){
        return *this;
    }
    delete[] data_;
    size_ = orig.size_;
    capacity_ = orig.capacity_;
    if(orig.size_ == 0){
        data_ = nullptr;
    }
    else{
        data_ = new char[capacity_];
        for(size_t i = 0; i<size_; ++i){
            data_[i] = orig.data_[i];
        }        
    }
    return *this;
}

String::~String(){
    delete[] data_;
}

char String::operator[](size_t k) const{
    return data_[k];
}
char& String::operator[](size_t k){
    return data_[k];
}

char* String::CStr() {
    return data_;
}
char* String::Data() {
    return data_;
}

const char* String::CStr() const{
    return data_;
}

const char* String::Data() const{
    return data_;
}
char& String::At(size_t k) const{
    if(k >= size_ ){
        throw StringOutOfRange{};
    }
    return data_[k];
}
char& String::At(size_t k){
    if(k >= size_){
        throw StringOutOfRange{};
    }
    return data_[k];
}

char& String::Front(){
    
    return data_[0];
}

const char& String::Front() const{
    return data_[0];
}

const char& String::Back() const{
    return data_[size_-1];
}

char& String::Back(){
    return data_[size_-1];
}

bool String::Empty() const{
    return size_ == 0;
}
size_t String::Size() const{
    return size_;
}
size_t String::Length() const{
    return size_;
}
size_t String::Capacity() const{
    return capacity_;
}
void String::Clear(){
    size_ = 0;
    if(data_ != nullptr){
        data_[0] = 0;
    }
}
void String::Swap(String& other){
    char* newdata = data_;
    data_ = other.data_;
    other.data_ = newdata;
    size_t newsize = size_;
    size_ = other.size_;
    other.size_ = newsize;
    size_t newcapacity = capacity_;
    capacity_ = other.capacity_;
    other.capacity_ = newcapacity;
}
void String::PopBack(){
    if(size_ == 0){
        return;
    }    
    size_--;
}
void String::PushBack(char symbol){
    if(data_ == nullptr){
        data_ = new char[1];
        data_[0] = symbol;        
        size_ = 1;
        capacity_ = 1;
    }
    else if (size_ < capacity_){
    	data_[size_] = symbol;
    	++size_;
    }
    else{
        Myrealloc(capacity_* 2);
		data_[size_] = symbol;
		++size_;        	        
    }
}
String& String::operator+=(const String& orig){

	size_t new_size = size_ + orig.size_;
	if(new_size==0){
		delete[] data_;
		data_ = nullptr;
		capacity_ = 0;
		size_ = 0;
	}
	else{

		if(new_size<=capacity_){
			for(size_t k = 0; k < orig.size_; ++k){
				data_[size_+k] = orig.data_[k];
			}
		}
		else{
			size_t newcap = new_size;
			if(capacity_*2 > newcap){
				newcap = capacity_*2;
			}
			Myrealloc(newcap);
			for(size_t k = 0; k < orig.size_; ++k){
				data_[size_+k] = orig.data_[k];
			}						
		}
		size_ = new_size;
	}
    
    return *this;
}
void String::Resize(size_t new_size , char symbol){
    if(new_size <= capacity_){
		if(new_size > size_){

			for(size_t k = 0; k < new_size-size_; ++k){
				data_[size_+k] = symbol;
			}
		}
        size_ = new_size;
    }
    else{
        Myrealloc(new_size);
        for(size_t k = 0; k < new_size-size_; ++k){
        	data_[size_+k] = symbol;
        }
        size_ = new_size;
    }
}
void String::Reserve(size_t new_capacity){
    if(new_capacity <= capacity_){
        return;
    }
    Myrealloc(new_capacity);    	
}

void String::ShrinkToFit(){
    /*if(data_ == nullptr || size_==capacity_){
        return;
    }
    if(size_==0){
    	data_ = nullptr;
    	capacity_ = 0;
    }
    else{
    	Myrealloc(size_);    
    }*/
    if(size_ < capacity_){
        this->Myrealloc(size_);
    }
}
String operator+(const String& other0, const String& other){
    String s = other0;    
    return s+=other;
}
bool operator<(const String& other0, const String& other ){
    size_t k = 0;
    while(k < other0.size_ && k < other.size_){
        if(other0.data_[k] < other.data_[k]){
            return true;
        }
        if(other0.data_[k] > other.data_[k]){
            return false;
        }
        k++;
    }
    return other0.size_ < other.size_;
}
bool operator>(const String& other0, const String& other){
    size_t k = 0;
    while(k < other0.size_ && k < other.size_){
        if(other0.data_[k] > other.data_[k]){
            return true;
        }
        if(other0.data_[k] < other.data_[k]){
            return false;
        }
        k++;
    }
    return other0.size_ > other.size_;

}
bool operator<=(const String& other0, const String& other){
    return !operator>(other0, other);
}
bool operator>=(const String& other0, const String& other){
    return !(operator<(other0, other));
}
bool operator==(const String& other0, const String& other){
    if(other0.size_ != other.size_){
        return false;
    }
    for(size_t i = 0; i<other0.size_; i++){
        if(other0.data_[i]!= other.data_[i]){
            return false;
        }
    }
    return true;
}
bool operator!=(const String& other0, const String& other){
    return !operator==(other0, other);
}

std::ostream& operator<<(std::ostream& os, const String& orig){
    if(orig.data_ == nullptr){
        return os;
    }
    for(size_t i = 0; i < orig.size_; i++){
        os << orig.data_[i];
    }
    return os;
}