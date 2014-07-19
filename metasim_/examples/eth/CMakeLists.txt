include_directories(../../src)

add_executable(eth
example.cpp
link.cpp
link.hpp
message.cpp
message.hpp
netinterface.cpp
netinterface.hpp
node.cpp
node.hpp
)

target_link_libraries(eth metasim)