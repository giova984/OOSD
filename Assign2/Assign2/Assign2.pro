TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Circle.cpp \
    CompositeShape.cpp \
    factory_registry.cpp \
    Rectangle.cpp \
    Shape.cpp \
    Triangle.cpp

HEADERS += \
    catch.hpp \
    Circle.h \
    CompositeShape.h \
    factory.h \
    Rectangle.h \
    Shape.h \
    Triangle.h

