#include "ex1.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool contains(const int &elem, const typename vector<int>::const_iterator &b, const typename vector<int>::const_iterator &e){
    for(auto it = b; it!=e; ++it)
        if (!(*it<elem) && !(elem<*it))
            return true;
    return false;
}

vector<int> intersection(const vector<int> &a, const vector<int> &b){
    vector<int> c;
    for (int i: a)
        if(contains(i,begin(b),end(b)) && !contains(i,begin(c),end(c)))
            c.insert(begin(c),i);
    return c;
}

vector<int> unione(const vector<int> &a, const vector<int> &b){
    vector<int> c;
    for (int i: a)
        if(!contains(i,begin(c),end(c))){
            c.insert(begin(c),i);
        }
    for (int i: b)
        if(!contains(i,c.begin(),c.end())){
            c.insert(begin(c),i);
        }
    return c;
}
