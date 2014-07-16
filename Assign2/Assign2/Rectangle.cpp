/*
 * Rect.cpp
 *
 *  Created on: Feb 20, 2011
 *      Author: lipari
 */

#include "Rectangle.h"
#include <iostream>
#include <sstream>
#include "factory.h"

using namespace std;

namespace {
registerInFactory<Shape, Rect> rr1("Rect");
registerInFactory<Shape, Rect> rr2("rect");
}

Rect::Rect(const point &center, float width, float height, const string &col) : Shape(), width(width), height(height), color(col)
{
    setCenter(center);
    cout << "Rectangle: " << getId() << " color: " << color << " center " << getCenter() << " width: " << width << " height: " << height << " Created" << endl;
}

Rect::~Rect() {
//    cout << "Destructor of Rectangle: " << getId() << endl;
}

void Rect::setWidth(float w){
    width = w;
}

void Rect::setHeight(float h){
    height = h;
}

void Rect::draw()
{
    cout << "Rectangle: " << i << " color: " << color << endl;
}
void Rect::resize(float scale)
{
    width *= scale;
    height *= scale;
    cout << "Rectangle: " << i << " resized to " << width << " x " << height <<endl;
}

void Rect::paint(const std::string &col){
    color = col;
}

std::string Rect::getColor(){
    return color;
}

Rect * Rect::createInstance(const string &parameters)
{
    point center(0,0);
    float width_ = 0;
    float height_ = 0;
    string color_("");

    std::stringstream ss(parameters);
    std::string param;
    while( ss >> param) {
        if (param == "center"){
            ss >> center.x;
            ss >> center.y;
        }
        else if (param == "width")
            ss >> width_;
        else if (param == "height")
            ss >> height_;
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
    return new Rect(center, width_, height_, color_);
}

void Rect::save(std::ostream &file){
    file << "Rect center " << getCenter().x << " " << getCenter().y << " width " << width << " height " << height << " color " << color << endl;
}
