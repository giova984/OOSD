include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(metasim
basestat.cpp entity.cpp genericvar.cpp regvar.cpp strtoken.cpp
trace.cpp debugstream.cpp event.cpp randomvar.cpp simul.cpp tick.cpp
)