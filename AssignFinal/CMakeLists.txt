cmake_minimum_required(VERSION 2.8.11)
project(Wifi)
find_path(METASIM_INCLUDE_DIR metasim.hpp)
find_library(METASIM_LIBRARY metasim)

add_definitions("-std=c++11")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

include_directories(${METASIM_INCLUDE_DIR})

add_executable(wifi 
node.cpp
message.cpp
link.cpp
netinterface.cpp
wifi.cpp
routing.cpp
example.cpp
)
target_link_libraries(wifi ${METASIM_LIBRARY} Qt5::Widgets)
