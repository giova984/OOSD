/*
 * Shape.cpp
 *
 *  Created on: May, 2014
 *      Author: avveduto
 */

#include "Shape.h"
#include <iostream>
using namespace std;
#include "CompositeShape.h"

int Shape::counter = 0;

Shape::Shape() : i(counter++), parent(nullptr){} // shape's default constructor

Shape::~Shape(){
    cout << "Destructor of Shape: " << i << endl;
    if(parent)
        setParent(nullptr);
}

void Shape::move(const point &p){
    center += p;
    cout << "Shape: " << i << " moved to " << center << endl;
}

void Shape::setParent(CompositeShape* p){
    // if previous parent remove shape from previous group
    if(parent){
        parent->dispose(this);
    }
    parent = p;
    if(parent)
        cout << "Shape: " << getId() << " added to group: " << parent->getId() << endl;

}

CompositeShape* Shape::getParent(){
    return parent;
}

void Shape::save(std::ostream&){ }



