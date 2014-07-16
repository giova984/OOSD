#include <iostream>

#ifndef MYCLASS_H
#define MYCLASS_H

class MyClass{
public:
    int val;
    MyClass(int v);
    bool operator<(const MyClass& other) const;

};

//global operators
//bool operator<(const MyClass &a,const MyClass &b);
std::ostream &operator<<(std::ostream& o, const MyClass &a);

#endif // MYCLASS_H
