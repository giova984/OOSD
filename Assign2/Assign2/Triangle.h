/*
 * Triangle.h
 *
 *  Created on: May, 2014
 *      Author: avveduto
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"

/*
 * Class representing a Triangle shape.
 * A triangle is defined by:
 * - center point
 * - length of the base which defines two corners (center +- [base/2 , 0])
 * - corner point relative to the base which represent the third corner
 * - color
*/
class Triangle : public Shape {
private:
    float base;
    point corner;
protected:
    std::string color;
    Triangle(const point &center,
             float base,
             const point& corner,
             const std::string &color); // Constructor of triangle
public:	
    static Triangle * createInstance(const std::string &color); // create an instance of a shape parsing a string param
    virtual ~Triangle();

    void setBase(float b); // set base size
    void setCorner(point &c); // set third corner's position (relative to the center)
    float getBase(){ return base; } // returns base size
    point getCorner(){ return corner; } // returns third corner point

    void draw(); // draws the triangle
    void resize(float scale); // scale the triangle by a float factor
    void paint(const std::string &col); // fill triangle's shape with a color
    std::string getColor(); // returns triangle shape's color

    void save(std::ostream &file); // save triangle as formatted string to an output stream
};

#endif /* TRIANGLE_H_ */
