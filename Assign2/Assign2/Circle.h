/*
 * Circle.h
 *
 *  Created on: May, 2014
 *      Author: avveduto
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Shape.h"

/*
 * Class representing a Circle shape.
 * A circle is defined by:
 * - center
 * - radius
 * - color
*/
class Circle: public Shape {
private:
    float radius;
protected:
    std::string color;
    Circle(const point &center,
           float radius,
           const std::string &col); // Constructor of Circle
public:
    static Circle *createInstance(const std::string &params); // create an instance of a shape parsing a string param
	virtual ~Circle();

    void setRadius(float r); // set circle's radius
    float getRadius(){ return radius; } // returns circle's radius

    void draw(); // draws the circle
    void resize(float scale); // scale the circle by a float factor
    void paint(const std::string &col); // fill circle's shape with a color
    std::string getColor(); // returns circle shape's color

    void save(std::ostream &file); // save circle as formatted string to an output stream
};

#endif /* CIRCLE_H_ */
