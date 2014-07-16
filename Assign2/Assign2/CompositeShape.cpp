#include "CompositeShape.h"
#include <iostream>
using namespace std;

CompositeShape::CompositeShape() : Shape()
{
    cout << "Group: " << Shape::getId() <<  " created" << endl;
}

CompositeShape::~CompositeShape()
{
    cout << "Group: " << getId() << " deleted" << endl;
    for (auto s : children) {
        s->setParent(nullptr);
    }
}

CompositeShape* CompositeShape::createInstance(){
    return new CompositeShape();
}

void CompositeShape::draw(){
    for(auto s : children)
            s->draw();
}

void CompositeShape::resize(float scale){
    for(auto s : children)
            s->resize(scale);
}

void CompositeShape::move(const point& p){
    cout << "Group: " << getId() << " move" << endl;
    for(auto s : children)
            s->move(p);
}

void CompositeShape::add(Shape* s){
    children.push_back(s);
    s->setParent(this);
}

// remove a Shape from children if belongs to group, without setting shape's parent
void CompositeShape::dispose(Shape* s){
    auto it = std::find(children.begin(), children.end(), s);
    if( it != children.end() ){
            cout << "Shape: " << s->getId() << " removed from group: " << getId() << endl;
            children.erase(it);
    }
}

// remove a Shape from children if belongs to group
void CompositeShape::remove(Shape *s){
    if( std::find(children.begin(), children.end(), s) != children.end() )
        s->setParent(nullptr);
}

// throws an exception instead of set centers of all shapes (design choice).
void CompositeShape::setCenter(const point&) { throw "\"setCenter\" unsupported on Group"; }

// throws an exception (design choice).
point CompositeShape::getCenter() { throw "\"getCenter\" unsupported on Group"; }


