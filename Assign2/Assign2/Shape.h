/*
 * Shape.h
 *
 *  Created on: May, 2014
 *      Author: avveduto
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <iostream>
#include <algorithm>
#include <vector>

class CompositeShape;

/*
 * Helper class representing a 2D point of float
*/
class point{
public:
    float x,y;
    point():x(0),y(0){}
    point(float x_,float y_):x(x_),y(y_){}
    point operator*(float f){
        point result(x*f, y*f);
        return result;
    }
    point& operator*=(float f){
        x *= f;
        y *= f;
        return *this;
    }
    point operator+(const point &o){
        point result(x+o.x, y+o.y);
        return result;
    }
    point& operator+=(const point &o){
        x += o.x;
        y += o.x;
        return *this;
    }
    bool operator==(const point &o){
        return x==o.x && y==o.y;
    }
    friend std::ostream &operator<<(std::ostream& o, const point &p){
        o << "(" << p.x << "," << p.y << ")";
        return o;
    }
};

/*
 * Abstract Shape class
*/
class Shape {
    static int counter;
protected:
    int i; //Shape's ID
    Shape();
    point center;
    CompositeShape *parent; //pointer to parent group. Each shape can belong to a single group

public:
    virtual ~Shape();
    virtual void draw() = 0;

    int getId(){ return i; } // return Shape's ID

    virtual void setParent(CompositeShape* p); // set Shape's parent, if a previous parent is present the Shape is removed from previous group.
    virtual CompositeShape* getParent(); //returns pointer to parent group

    virtual void setCenter(const point &p){ center = p; } //set shape's center
    virtual point getCenter(){ return center; } // returns shape's center

    virtual void resize(float scale) = 0; // scale shape's size by a float factor
    virtual void move(const point &p); // translate shape's center by a point factor

    virtual void save(std::ostream &o); // save shape as formatted string to an output stream
};



#endif /* SHAPE_H_ */
