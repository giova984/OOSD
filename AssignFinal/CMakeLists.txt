cmake_minimum_required(VERSION 2.8.11)
project(Wifi)
find_path(METASIM_INCLUDE_DIR metasim.hpp)
find_library(METASIM_LIBRARY metasim)

add_definitions("-std=c++11")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5PrintSupport)

include_directories(${METASIM_INCLUDE_DIR})

#add_subdirectory(qcustomplot)

qt5_wrap_ui(MAIN_UI wifi.ui)
qt5_wrap_ui(PLOTRESULTS_UI plotresults.ui)



add_executable(wifi
${MAIN_UI}
${PLOTRESULTS_UI}
node.cpp
message.cpp
link.cpp
netinterface.cpp
routing.cpp
main.cpp
nodeviewer.cpp
wifi.cpp
wifistatistics.cpp
qcustomplot.h qcustomplot.cpp
plotresults.cpp
)
target_link_libraries(wifi ${METASIM_LIBRARY} Qt5::Widgets Qt5::PrintSupport)

