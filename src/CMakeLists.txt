set(BINARY ${CMAKE_PROJECT_NAME})
set(SOURCES
	Camera.cpp
	World.cpp
	Material.cpp
	Shape.cpp
	Ray.cpp
	Transformation.cpp
	Matrix.cpp
	Tuple.cpp
	Color.cpp
	Canvas.cpp)

add_library(${BINARY}_lib STATIC ${SOURCES})

target_link_libraries(${BINARY}_lib PRIVATE project_warnings)
#target_link_libraries(${BINARY}_lib PRIVATE --coverage)

# Link OpenMP
find_package(OpenMP)
target_link_libraries(${BINARY}_lib PRIVATE OpenMP::OpenMP_CXX)