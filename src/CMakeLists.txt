set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES Camera.cpp World.cpp Material.cpp Sphere.cpp Ray.cpp Transformation.cpp Matrix.cpp Tuple.cpp Color.cpp Canvas.cpp main.cpp)

add_executable(${BINARY}_run ${SOURCES})

#add_library(${BINARY} _lib STATIC ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES})

target_link_libraries(${BINARY}_run PRIVATE project_warnings)

# Link OpenMP
find_package(OpenMP)
target_link_libraries(${BINARY}_run PUBLIC OpenMP::OpenMP_CXX)