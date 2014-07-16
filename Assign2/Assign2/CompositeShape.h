#ifndef COMPOSITESHAPE_H
#define COMPOSITESHAPE_H

#include "Shape.h"

/*
 * class representing a group.
 * Derived from Shape.
 * Operations performed on the group are performed on all contained shapes (move, resize, draw)
*/
class CompositeShape
        : public Shape
{
private:
    std::vector<Shape*> children; // Naked pointers to grouped shapes. Ownership is mantained by the main program.
    void dispose(Shape* s); // Signals to the group that a shape had to be removed from the group.
    friend void Shape::setParent(CompositeShape*); // In this way Shape can use dispose method, without make it public and usable in wrong ways
protected:
    CompositeShape(); // Default constrauctor to create a void group.
public:
    static CompositeShape * createInstance(); // function to create an instance of a Shape.

    virtual ~CompositeShape();

    void setCenter(const point &p); // throws an exception instead of set centers of all shapes (design choice).
    point getCenter(); // throws an exception (design choice).
    void draw(); // draws all shapes belonging the group
    void resize(float scale); // resize all shapes belonging the group by a float factor
    void move(const point &p); // translate all shapes belonging the group by a point factor

    void add(Shape *s); // add a shape to the group
    void remove(Shape *s); // remove a shape from the group (shape is not deleted)
    int getNumChild(){ return children.size(); } // returns the number of shapes in the group

};

#endif // COMPOSITESHAPE_H
