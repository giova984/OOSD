/*
 * Rect.h
 *
 *  Created on: May, 2014
 *      Author: avveduto
 */

#ifndef Rect_H_
#define Rect_H_

#include "Shape.h"

/*
 * Class representing a Rectangle shape.
 * A rectangle is defined by:
 * - center point
 * - width
 * - height
 * - color
*/
class Rect : public Shape {
private:
    float width, height;
protected:
	std::string color;
    Rect(const point &center,
         float width,
         float height,
         const std::string &color); //Constructor of rectangle
public:
    static Rect * createInstance(const std::string &s); // create an instance of a shape parsing a string param
    virtual ~Rect();

    void setWidth(float w); // set reactangle's width
    void setHeight(float h); // set rectangle's height
    float getWidth(){return width;} // returns rectangle's width
    float getHeight(){return height;} // returns rectangle's height

    void draw(); //draws the rectangle
    void resize(float scale); // scale the rectangle by a float factor
    void paint(const std::string &col); // fill rectangle's shape with a color
    std::string getColor(); // returns rectangle shape's color

    void save(std::ostream &file);// save rectangle as formatted string to an output stream
};

#endif /* Rect_H_ */
