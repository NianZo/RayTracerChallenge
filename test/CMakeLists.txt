add_executable(UnitTests main.cpp TupleTest.cpp ColorTest.cpp CanvasTest.cpp)

add_test(NAME UnitTests COMMAND UnitTests)

# add_library(gtest gtest_main)
target_link_libraries(UnitTests ${CMAKE_PROJECT_NAME}_lib gtest)