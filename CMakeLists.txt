cmake_minimum_required (VERSION 3.15)

project (CMakeTest)

# add_executable(CMakeTest CMakeTest.cpp)

option(ENABLE_TESTING "Enable Test Builds" ON)

add_subdirectory(src)

if(ENABLE_TESTING)
  message("Building tests.")
  add_subdirectory(test)
endif()

