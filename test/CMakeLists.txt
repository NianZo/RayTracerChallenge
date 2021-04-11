add_executable(UnitTests main.cpp TupleTest.cpp ColorTest.cpp CanvasTest.cpp MatrixTest.cpp TransformationTest.cpp)

# target_link_libraries(UnitTests PUBLIC "${CMAKE_PROJECT_NAME}_lib" gtest)
target_link_libraries(UnitTests PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

add_test(NAME UnitTests COMMAND UnitTests) # Should add UnitTests to the list of tests to be run when ctest is run

# add_library(gtest gtest_main)
# target_link_libraries(UnitTests PUBLIC "${CMAKE_PROJECT_NAME}_lib" gtest)
# target_include_directories(UnitTests PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}")