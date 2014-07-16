#include <iostream>
#include <vector>
#include <list>
#include <exception>
//#include "ex1.hpp"
//#include "ex2.hpp"
#include "ex3.hpp"
#include "MyClass.hpp"

using namespace std;


template<class T>
void print(const T &begin, const T &end)
{
    if(begin == end) {
        cout<<"{ }"<<endl;
        return;
    }
    T i = begin;
    std::cout << "{";
    if (i!=end) cout << *i;
    i++;
    for (;i!=end; i++) cout << ", " << *i;
    cout << "}" << endl;
}

template <class Container>
void print(const Container &c) {
    cout << "{";
    for(typename Container::const_iterator it = begin(c); it!=end(c); ){
        cout << *it;
        it++;
        if((it)!=end(c))
            cout << ",";
    }
    cout << "}"<<endl;
}

int main()
{
    vector<int> vi1;
    vector<int> vi2;
    vector<MyClass> vm1;
    vector<MyClass> vm2;

    list<int> li1;
    list<int> li2;
    list<MyClass> lm1;
    list<MyClass> lm2;

    for(int i=0; i<10;i++){
        vi1.push_back(i);
        vm1.push_back(i);
        li1.push_back(i);
        lm1.push_back(i);
    }
    for(int i=4; i<8;i++){
        vi1.push_back(i);
        vm1.push_back(i);
        li1.push_back(i);
        lm1.push_back(i);
    }
    for (int i=6; i<15; i++){
        vi2.push_back(i);
        vm2.push_back(i);
        li2.push_back(i);
        lm2.push_back(i);
    }
    for (int i=8; i<10; i++){
        vi2.push_back(i);
        vm2.push_back(i);
        li2.push_back(i);
        lm2.push_back(i);
    }

    cout << "vector<int>" <<endl;
    print(vi1);
    print(vi2);
    cout << "intersection" <<endl;
    vector<int> vi3;
    vi3 = intersection(vi1,vi2);
    print(vi3);
    cout << "union" <<endl;
    vi3 = unione(vi1,vi2);
    print(vi3);
    cout << endl;
    cout << "vector<MyClass>" <<endl;
    print(vm1);
    print(vm2);
    vector<MyClass> vm3;
    vm3 = intersection(vm1,vm2);
    cout << "intersection" <<endl;
    print(vm3);
    vm3 = unione(vm1,vm2);
    cout << "union" <<endl;
    print(vm3);
    cout << endl;
    cout << endl;


    cout << "list<int>" <<endl;
    print(li1);
    print(li2);
    cout << "intersection" <<endl;
    list<int> li3;
    li3 = intersection(li1,li2);
    print(li3);
    cout << "union" <<endl;
    li3 = unione(li1,li2);
    print(li3);
    cout << endl;
    cout << "list<MyClass>" <<endl;
    print(lm1);
    print(lm2);
    list<MyClass> lm3;
    lm3 = intersection(lm1,lm2);
    cout << "intersection" <<endl;
    print(lm3);
    lm3 = unione(lm1,lm2);
    cout << "union" <<endl;
    print(lm3);
    cout << endl;
    cout << endl;

//    //list<int> li3_1 = {1,2};
//    list<int> li3_1(li3);
//    //list<int> li3_2 = {1,2};
//    list<int> li3_2(li3);

//    auto it1 = li3_1.begin();
//    auto it12 = li3_1.begin();
//    it12++;
//    //it1++;
//    li3_1.erase(it1++);
//    //li3_1.erase(it12);
//    print(li3_1);

////    auto it2 = li3_2.begin();
////    li3_2.erase(it2);
////    it2++;
////    li3_2.erase(it2);
////    print(li3_2);


    return 0;
}


