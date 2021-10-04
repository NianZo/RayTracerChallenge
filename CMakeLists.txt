cmake_minimum_required (VERSION 3.15)

project (CMakeTest)

# set_property(TARGET tgt PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

# Standard compiler warnings
add_library(project_warnings INTERFACE)

include(CompilerWarnings.cmake)
set_project_warnings(project_warnings)
# add_executable(CMakeTest CMakeTest.cpp)
# target_compile_features(project_options INTERFACE cxx_std_20)

option(ENABLE_TESTING "Enable Test Builds" ON)

add_subdirectory(src)

if(ENABLE_TESTING)
  message("Building tests.")
  add_subdirectory(test)
endif()

