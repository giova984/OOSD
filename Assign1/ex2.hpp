#include <vector>
#include <algorithm>

#ifndef EX2_HPP
#define EX2_HPP

using namespace std;

template<class T>
bool contains(T &elem, const typename vector<T>::const_iterator &b, const typename vector<T>::const_iterator &e){
    for(auto it = b; it!=e; ++it)
        if (!(*it<elem) && !(elem<*it))
            return true;
    return false;
}

template<class T>
vector<T> intersection(const vector<T> &a, const vector<T> &b){
    vector<T> c;
    for (T i: a)
        if(contains(i,begin(b),end(b)) && !contains(i,begin(c),end(c)))
            c.insert(begin(c),i);
    return c;
}

template<class T>
vector<T> unione(const vector<T> &a, const vector<T> &b){
    vector<T> c;
    for (T i: a)
        if(!contains(i,begin(c),end(c))){
            c.insert(begin(c),i);
        }
    for (T i: b)
        if(!contains(i,begin(c),end(c))){
            c.insert(begin(c),i);
        }
    return c;
}

#endif
