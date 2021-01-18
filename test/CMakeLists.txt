add_executable(UnitTests TupleTest.cpp main.cpp)

add_test(NAME UnitTests COMMAND UnitTests)

# add_library(gtest gtest_main)
target_link_libraries(UnitTests ${CMAKE_PROJECT_NAME}_lib gtest)