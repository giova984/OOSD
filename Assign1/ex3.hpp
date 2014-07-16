#include <algorithm>

#ifndef EX3_HPP
#define EX3_HPP

using namespace std;

template<class E, class T>
bool contains(E &elem, const T &b, const T &e){
    for(auto it = b; it!=e; ++it)
        if (!(*it<elem) && !(elem<*it))
            return true;
    return false;
}

template<class T>
T intersection(const T &a, const T &b){
    T c;
    for (decltype(*begin(a)) i: a)
        if(contains(i,begin(b),end(b)) && !contains(i,begin(c),end(c)))
            c.insert(begin(c),i);
    return c;
}

template<class T>
T unione(const T &a, const T &b){
    T c;
    for (decltype(*begin(a)) i: a)
        if(!contains(i,begin(c),end(c))){
            c.insert(begin(c),i);
        }
    for (auto i: b)
        if(!contains(i,begin(c),end(c))){
            c.insert(begin(c),i);
        }
    return c;
}

#endif
