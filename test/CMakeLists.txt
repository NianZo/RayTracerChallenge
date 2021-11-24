set(TEST_SOURCES main.cpp TupleTest.cpp ColorTest.cpp CanvasTest.cpp MatrixTest.cpp TransformationTest.cpp RayTest.cpp SphereTest.cpp LightTest.cpp MaterialTest.cpp WorldTest.cpp CameraTest.cpp PatternTest.cpp)
add_executable(UnitTests ${TEST_SOURCES})
target_include_directories(UnitTests PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(UnitTests PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)
target_link_libraries(UnitTests PRIVATE --coverage)

add_test(NAME UnitTests COMMAND UnitTests)

# add_library(gtest gtest_main)
# target_link_libraries(UnitTests PUBLIC "${CMAKE_PROJECT_NAME}_lib" gtest)
# target_include_directories(UnitTests PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}")