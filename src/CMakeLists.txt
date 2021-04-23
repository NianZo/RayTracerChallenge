set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES Ray.cpp Transformation.cpp Matrix.cpp Tuple.cpp Color.cpp Canvas.cpp main.cpp)

add_executable(${BINARY}_run ${SOURCES})

# add_library(${BINARY}_lib STATIC ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES})