/*
 * Circle.cpp
 *
 *  Created on: Mar 16, 2011
 *      Author: lipari
 */

#include "Circle.h"
#include "factory.h"
#include <iostream>
#include <sstream>

using namespace std;

namespace {
registerInFactory<Shape, Circle> rc1("circle");
registerInFactory<Shape, Circle> rc2("Circle");
}

Circle::Circle(const point &center, float radius, const std::string &col) : Shape(), radius(radius), color(col)
{
    setCenter(center);
    cout << "Circle: " << getId() << " color: " << color << " center " << getCenter() << " radius: " << radius << " Created" << endl;
}

Circle::~Circle()
{
//    cout << "Destructor of Circle: " << getId() << endl;
}

Circle * Circle::createInstance(const string &parameters = "")
{
    point center(0,0);
    float radius_ = 0;
    string color_("");

    std::stringstream ss(parameters);
    std::string param;
    while( ss >> param) {
        if (param == "center"){
            ss >> center.x;
            ss >> center.y;
        }
        else if (param == "radius")
            ss >> radius_;
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
    return new Circle(center, radius_, color_);
}

void Circle::setRadius(float r){
    radius = r;
}

void Circle::draw()
{
    cout << "This is a circle, color " << color << endl;
}

void Circle::resize(float scale)
{
    radius *= scale;
    cout << "Circle: " << i << " resized, radius: " << radius <<endl;
}

void Circle::paint(const std::string &col){
    color = col;
}

std::string Circle::getColor(){
    return color;
}

void Circle::save(std::ostream &file){
    file << "Circle center " << getCenter().x << " " << getCenter().y << " radius " << radius << " color " << color << endl;
}
