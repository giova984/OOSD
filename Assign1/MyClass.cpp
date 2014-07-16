#include "MyClass.hpp"

MyClass::MyClass(int v):val(v){};

bool MyClass::operator<(const MyClass &other) const{
    return val < other.val;
}

//bool operator<(const MyClass &first,const MyClass &other){
//    return first.val < other.val;
//}

std::ostream &operator<<(std::ostream& o, const MyClass &a){
    //o << "[" << a.val << "]";
    o << a.val;
    return o;
}
