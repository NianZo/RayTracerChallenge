set(TEST_BINARY "UnitTests")
set(TEST_SOURCES 
	main.cpp
	TupleTest.cpp
	ColorTest.cpp
	CanvasTest.cpp
	MatrixTest.cpp
	TransformationTest.cpp
	RayTest.cpp
	ShapeTest.cpp
	LightTest.cpp
	MaterialTest.cpp
	WorldTest.cpp
	CameraTest.cpp
	PatternTest.cpp)

add_executable(${TEST_BINARY} ${TEST_SOURCES})
target_include_directories(${TEST_BINARY} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${TEST_BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

if(CMAKE_BUILD_TYPE MATCHES Debug)
	target_link_libraries(${TEST_BINARY} PRIVATE --coverage)
endif()


add_test(NAME ${TEST_BINARY} COMMAND ${TEST_BINARY})
