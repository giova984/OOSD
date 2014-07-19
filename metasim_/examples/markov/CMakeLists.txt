include_directories(../../src)

add_executable(markov
markov.cpp
)

target_link_libraries(markov metasim)