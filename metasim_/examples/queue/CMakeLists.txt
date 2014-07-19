include_directories(../../src)

add_executable(queue
queue.cpp
)

target_link_libraries(queue metasim)