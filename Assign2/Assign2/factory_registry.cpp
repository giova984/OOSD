//============================================================================
// Name        : factory_registry.cpp
// Author      : Giuseppe Lipari
// Version     :
// Copyright   : c
// Description : Hello World in C++, Ansi-style
//============================================================================

#define CATCH_CONFIG_RUNNER

#include <iostream>
#include <fstream>
using namespace std;
#include "Shape.h"
#include "CompositeShape.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Circle.h"
#include "factory.h"
#include "catch.hpp"

#define GFACT genericFactory<Shape>::instance()

unique_ptr<Shape> load_object(istream &file);
vector<unique_ptr<Shape>> load(istream &file) throw();

TEST_CASE("Tests on Factory") {

    SECTION("Checking correctness of the factory method"){

        Shape* s_tmp = GFACT.create("triangle", "center 5 6 base 12 corner 5 5 color red").release();
        unique_ptr<Triangle> t1(dynamic_cast<Triangle*>(s_tmp));
        s_tmp =  GFACT.create("rect", "center 7 8 width 8 height 5 color blue").release();
        unique_ptr<Rect> r1(dynamic_cast<Rect*>(s_tmp));
        s_tmp =  GFACT.create("circle", "center 9 10 radius 17 color orange").release();
        unique_ptr<Circle> c1(dynamic_cast<Circle*>(s_tmp));

        REQUIRE( t1->getCenter() == point(5, 6) );
        REQUIRE( t1->getBase() == 12 );
        REQUIRE( t1->getCorner() == point(5, 5) );
        REQUIRE( t1->getColor() == "red" );

        REQUIRE( r1->getCenter() == point(7, 8) );
        REQUIRE( r1->getWidth() == 8 );
        REQUIRE( r1->getHeight() == 5 );
        REQUIRE( r1->getColor() == "blue");

        REQUIRE( c1->getCenter() == point(9, 10) );
        REQUIRE( c1->getRadius() == 17 );
        REQUIRE( c1->getColor() == "orange" );
    }

    SECTION("Checking thrown of exception in case of wrong parameters of factory method"){
        unique_ptr<Shape> up(GFACT.create("trian", "center 5 6 base 12 corner 5 5 color red"));
        std::cout << "trian "  << std::endl;
        REQUIRE_NOTHROW( GFACT.create("triangle", "center 5 6 base 12 corner 5 5 color red") );
        REQUIRE_NOTHROW( GFACT.create("triangle", "") );
        REQUIRE_NOTHROW( GFACT.create("triangle", "base 12 corner 5 5 color red") );
        REQUIRE_NOTHROW( GFACT.create("triangle", "center 5 6 corner 5 5 color red") );
        REQUIRE_NOTHROW( GFACT.create("triangle", "center 5 6 base 12 color red") );
        REQUIRE_THROWS( GFACT.create("triangle", "center 5 base 12 corner 5 5 color red") );
        REQUIRE_THROWS( GFACT.create("triangle", "center base 12 corner 5 5 color red") );
        REQUIRE_THROWS( GFACT.create("triangle", "center 5 6 base corner 5 5 color red") );
        REQUIRE_THROWS( GFACT.create("triangle", "center 5 6 12 corner 5 5 color red") );
        REQUIRE_THROWS( GFACT.create("triangle", "center 5 6 base 12 corner red") );
        REQUIRE_THROWS( GFACT.create("triangle", "center 5 6 base 12 corner 5 5 color red brown") );
        REQUIRE_THROWS( GFACT.create("triangle", "ciccio 5 6 base 12 corner 5 5 color red") );
    }
}

TEST_CASE("Tests on Shapes functions"){

    Shape* s_tmp = GFACT.create("triangle", "").release();
    unique_ptr<Triangle> t1(dynamic_cast<Triangle*>(s_tmp));
    s_tmp =  GFACT.create("rect", "").release();
    unique_ptr<Rect> r1(dynamic_cast<Rect*>(s_tmp));
    s_tmp =  GFACT.create("circle", "").release();
    unique_ptr<Circle> c1(dynamic_cast<Circle*>(s_tmp));

    float base = 10;
    point corner(1,10);
    float radius = 7;
    float width = 13;
    float height = 9;
    std::string color = "orange";

    point center(10,10);
    point translation(5,5);
    point center_translated = center + translation;
    float scale = 0.7;
    float base_scaled = base * scale;
    point corner_scaled = corner * scale;
    float radius_scaled = radius * scale;
    float width_scaled = width * scale;
    float height_scaled = height * scale;

    SECTION( "Tests on Triangle" ){
        t1->setCenter(center);
        t1->setBase(base);
        t1->setCorner(corner);
        REQUIRE( t1->getCenter() == center );
        REQUIRE( t1->getBase() == base );
        REQUIRE( t1->getCorner() == corner );

        t1->move(translation);
        REQUIRE( t1->getCenter() == center_translated );
        REQUIRE( t1->getBase() == base );
        REQUIRE( t1->getCorner() == corner );

        t1->resize(scale);
        REQUIRE( t1->getCenter() == center_translated );
        REQUIRE( t1->getBase() == base_scaled );
        REQUIRE( t1->getCorner() == corner_scaled );

        t1->paint(color);
        REQUIRE( t1->getColor().compare(color) == 0 );
    }
    SECTION("Tests on Circle"){
        c1->setCenter(center);
        c1->setRadius(radius);
        REQUIRE( c1->getCenter() == center );
        REQUIRE( c1->getRadius() == radius );

        c1->move(translation);
        REQUIRE( c1->getCenter() == center_translated );
        REQUIRE( c1->getRadius() == radius );

        c1->resize(scale);
        REQUIRE( c1->getCenter() == center_translated );
        REQUIRE( c1->getRadius() == radius_scaled );

        c1->paint(color);
        REQUIRE( c1->getColor().compare(color) == 0 );
    }
    SECTION("Tests on Rectangle"){
        r1->setCenter(center);
        r1->setWidth(width);
        r1->setHeight(height);
        REQUIRE( r1->getCenter() == center );
        REQUIRE( r1->getWidth() == width );
        REQUIRE( r1->getHeight() == height );

        r1->move(translation);
        REQUIRE( r1->getCenter() == center_translated );
        REQUIRE( r1->getWidth() == width );
        REQUIRE( r1->getHeight() == height );

        r1->resize(scale);
        REQUIRE( r1->getCenter() == center_translated );
        REQUIRE( r1->getWidth() == width_scaled );
        REQUIRE( r1->getHeight() == height_scaled );

        r1->paint(color);
        REQUIRE( r1->getColor().compare(color) == 0 );
    }

}

TEST_CASE("Tests on Groups"){

    vector<unique_ptr<Shape>> shapes;
    for(int i=0; i<5; i++){
        shapes.push_back( GFACT.create("triangle", "center 0 0 base 12 corner 5 5 color red") );
        shapes.push_back( GFACT.create("rect", "center 0 0 width 8 height 5 color blue") );
        shapes.push_back( GFACT.create("circle", "center 0 0 radius 17 color orange") );
    }

    unique_ptr<CompositeShape> g1( CompositeShape::createInstance() );
    unique_ptr<CompositeShape> g2( CompositeShape::createInstance() );
    unique_ptr<CompositeShape> g3( CompositeShape::createInstance() );

    SECTION("Testing group exceptions"){
        REQUIRE_THROWS( g1->setCenter(point(5,5)) );
        REQUIRE_THROWS( g1->getCenter());
    }

    SECTION("Testing add to group and remove from group"){
        for(int i=0; i<5; i++){
            g1->add(shapes[i*3].get());
            g2->add(shapes[i*3+1].get());
            g3->add(shapes[i*3+2].get());
        }
        REQUIRE ( g1->getNumChild() == 5 );
        REQUIRE ( g2->getNumChild() == 5 );
        REQUIRE ( g3->getNumChild() == 5 );

        for(int i=0; i<5; i++){
            REQUIRE( shapes[i*3].get()->getParent() == g1.get() );
            REQUIRE( shapes[i*3+1].get()->getParent() == g2.get() );
            REQUIRE( shapes[i*3+2].get()->getParent() == g3.get() );
        }

        g1->remove(shapes[0].get());
        REQUIRE( shapes[0].get()->getParent() == nullptr );
        REQUIRE ( g1->getNumChild() == 4 );
        g1->remove(shapes[3].get());
        REQUIRE( shapes[3].get()->getParent() == nullptr );
        REQUIRE ( g1->getNumChild() == 3 );
    }

    for(int i=0; i<5; i++){
        g1->add(shapes[i*3].get());
        g2->add(shapes[i*3+1].get());
        g3->add(shapes[i*3+2].get());
    }

    SECTION("Testing automatic remove from group on Shape Destruction"){

        Shape* sh = GFACT.create("triangle", "center 0 0 base 12 corner 5 5 color red").release();
        g1->add(sh);
        REQUIRE( g1->getNumChild() == 6 );
        REQUIRE_NOTHROW( delete(sh) );
        REQUIRE( g1->getNumChild() == 5 );
    }

    SECTION("Removing wrong or null child"){
      CompositeShape *g1_p = shapes[1].get()->getParent();
        REQUIRE_NOTHROW( g1->remove(shapes[1].get()) );
        REQUIRE( shapes[1].get()->getParent() == g1_p );
        REQUIRE ( g1->getNumChild() == 5 );
        REQUIRE_NOTHROW( g1->remove(nullptr) );
        REQUIRE ( g1->getNumChild() == 5 );
    }

    SECTION("Testing nested group parent"){
        g1->add(g2.get());
        for(int i=0; i<5; i++){
            REQUIRE( g2->getParent() == g1.get() );
        }
    }

    SECTION("Testing functions on groups"){
        g1->move(point(2, 2));
        for(int i=0; i<5; i++){
            REQUIRE( shapes[i*3].get()->getCenter() == point(2, 2) );
            REQUIRE( shapes[i*3+1].get()->getCenter() == point(0, 0) );
            REQUIRE( shapes[i*3+2].get()->getCenter() == point(0, 0) );
        }

        g1->resize(0.5); //Rectangles and circles not resized
        for(int i=0; i<15; i++){
            if(Triangle* t = dynamic_cast<Triangle*>(shapes[i].get())){
                REQUIRE( t->getBase() == 6 );
                REQUIRE( t->getCorner() == point(2.5, 2.5) );
            }else if(Rect* t = dynamic_cast<Rect*>(shapes[i].get())){
                REQUIRE( t->getWidth() == 8 );
                REQUIRE( t->getHeight() == 5 );
            }else if(Circle* t = dynamic_cast<Circle*>(shapes[i].get())){
                REQUIRE( t->getRadius() == 17 );
            }
        }
    }


    SECTION("Testing functions on nested groups"){
        g1->add(g2.get());
        g2->add(g3.get());
        g1->move(point(2, 2));
        for(int i=0; i<5; i++){
            REQUIRE( shapes[i*3].get()->getCenter() == point(2, 2) );
            REQUIRE( shapes[i*3+1].get()->getCenter() == point(2, 2) );
            REQUIRE( shapes[i*3+2].get()->getCenter() == point(2, 2) );
        }
        g1->resize(0.5);
        for(int i=0; i<15; i++){
            if(Triangle* t = dynamic_cast<Triangle*>(shapes[i].get())){
                REQUIRE( t->getBase() == 6 );
                REQUIRE( t->getCorner() == point(2.5, 2.5) );
            }else if(Rect* t = dynamic_cast<Rect*>(shapes[i].get())){
                REQUIRE( t->getWidth() == 4 );
                REQUIRE( t->getHeight() == 2.5 );
            }else if(Circle* t = dynamic_cast<Circle*>(shapes[i].get())){
                REQUIRE( t->getRadius() == 8.5 );
            }
        }
    }

    SECTION("Testing move group"){
        g1->move(point(2, 2));
        for(int i=0; i<5; i++){
            REQUIRE( shapes[i*3].get()->getCenter() == point(2, 2) );
            REQUIRE( shapes[i*3+1].get()->getCenter() == point(0, 0) );
            REQUIRE( shapes[i*3+2].get()->getCenter() == point(0, 0) );
        }
    }

    SECTION("Testing resize nested groups"){
        g1->add(g2.get());
        g1->move(point(2, 2));
        for(int i=0; i<5; i++){
            REQUIRE( shapes[i*3].get()->getCenter() == point(2, 2) );
            REQUIRE( shapes[i*3+1].get()->getCenter() == point(2, 2) );
            REQUIRE( shapes[i*3+2].get()->getCenter() == point(0, 0) );
        }
    }
}



TEST_CASE("Tests on saving and loading Shapes on a file") {

    SECTION("Saving Spapes on a file"){
        unique_ptr<Shape> s1 = GFACT.create("triangle", "center 6 6 base 12 corner 5 5 color red");
        unique_ptr<Shape> s2 = GFACT.create("rect", "center 7 7 width 8 height 5 color blue");
        unique_ptr<Shape> s3 = GFACT.create("circle", "center 8 8 radius 17 color orange");

        std::filebuf fb;
        fb.open ("save.txt",std::ios::out);
        std::ostream os(&fb);
        s1->save(os);
        s2->save(os);
        s3->save(os);
        fb.close();

        fb.open ("save.txt",std::ios::in);
        std::string str;
        std::istream is(&fb);
        getline(is,str);
        REQUIRE( str == "Triangle center 6 6 base 12 corner 5 5 color red" );
        getline(is,str);
        REQUIRE( str == "Rect center 7 7 width 8 height 5 color blue" );
        getline(is,str);
        REQUIRE( str == "Circle center 8 8 radius 17 color orange" );
        fb.close();
    }

    SECTION("Loading Shapes from file"){
        std::filebuf fb;
        fb.open ("save.txt",std::ios::in);
        std::istream is(&fb);

        Shape* sh(load_object(is).release());
        REQUIRE( dynamic_cast<Triangle*>(sh) );
        delete(sh);
        sh = (load_object(is).release());
        REQUIRE( dynamic_cast<Rect*>(sh) );
        delete(sh);
        sh = (load_object(is).release());
        REQUIRE_FALSE( dynamic_cast<Rect*>(sh) );
        REQUIRE( dynamic_cast<Circle*>(sh) );
        delete(sh);

        fb.close();
    }

    SECTION("Tests global load function"){
        std::filebuf fb;
        fb.open ("save.txt",std::ios::in);
        std::istream is(&fb);

        vector<unique_ptr<Shape>> shapes;
        REQUIRE_NOTHROW( shapes = load(is) );

        REQUIRE( shapes.size() == 3 );
        fb.close();
    }
}

int main( int argc, char* const argv[] )
{
   //Running CATCH tests
  Catch::Session session;
  int returnCode = session.applyCommandLine( argc, argv );
  if( returnCode != 0 )
    return returnCode;
  return session.run();
}

/*
 * global load object function which reads a line from an input
 * stream and uses the factory to return a Shape
 * throws an exception if the string is not well formed
*/
unique_ptr<Shape> load_object(istream &file){
    if(file.peek() == EOF)
        return nullptr;
    std::string type;
    std::string params;
    file >> type;
    getline(file, params);
    cout << type << " , " << params << endl;
    return GFACT.create(type,params);
}
/*
 * global load function which reads all lines from an input
 * stream and uses the factory to return a vector of shapes
 * containing read shapes until an EOF is found or a
 * string is not well formed
 * throws no exceptions
*/
vector<unique_ptr<Shape>> load(istream &file) throw(){
    vector<unique_ptr<Shape>> shapes;
    try{
        while(file.peek() != EOF)
            shapes.push_back(load_object(file));
    }catch(...){ }
    return shapes;
}

