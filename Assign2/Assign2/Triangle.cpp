/*
 * Triangle.cpp
 *
 *  Created on: Feb 20, 2011
 *      Author: lipari
 */

#include "Triangle.h"
#include <iostream>
#include <sstream>
#include "factory.h"
using namespace std;

namespace {
registerInFactory<Shape, Triangle> tr1("triangle");
registerInFactory<Shape, Triangle> tr2("Triangle");
}

Triangle::Triangle(const point &center, float base, const point& corner, const std::string &color) :
    Shape(),
    base(base),
    corner(corner),
    color(color)
{
    setCenter(center);
    cout << "Triangle: " << getId() << " color: " << color << " center " << getCenter() << " base: " << base << " corner: " << corner << " Created" << endl;
}

Triangle::~Triangle()
{
//    cout << "Destructor of Triangle: " << getId() << endl;
}

void Triangle::setBase(float b){
    base = b;
}

void Triangle::setCorner(point &c){
    corner.x = c.x;
    corner.y = c.y;
}

void Triangle::draw()
{
    cout << "Draw Triangle: " << i << " color: " << color << endl;
}
void Triangle::resize(float scale)
{
    base *= scale;
    corner *= scale;
    cout << "Triangle: " << i << " resized, base: "
            << base
            << " corner: " << corner << endl;
}

void Triangle::paint(const std::string &col){
    color = col;
}

std::string Triangle::getColor(){
    return color;
}

Triangle * Triangle::createInstance(const string &parameters)
{
    point center(0,0);
    float base_ = 0;
    point corner_(0,0);
    string color_("");

    std::stringstream ss(parameters);
    std::string param;
    while( ss >> param) {
        if (param == "center"){
            ss >> center.x;
            ss >> center.y;
        }
        else if (param == "base")
            ss >> base_;
        else if (param == "corner"){
            ss >> corner_.x;
            ss >> corner_.y;
        }
        else if (param == "color")
            ss >> color_;
        else {
            std::stringstream err;
            err << "unknown parameter type: \"" << param << "\"";
            throw std::runtime_error(err.str());
        }
        if (!ss) {
            std::stringstream err;
            err << "error parsing parameter: \"" << param << "\"";
            throw std::runtime_error(err.str());
        }
    }

     return new Triangle(center, base_,corner_,color_);

}

void Triangle::save(std::ostream &file){
    file << "Triangle center " << getCenter().x << " " << getCenter().y << " base " << base << " corner " << corner.x << " " << corner.y << " color " << color << endl;
}
