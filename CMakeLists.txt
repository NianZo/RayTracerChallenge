cmake_minimum_required(VERSION 3.15)

project(RayTracerChallenge)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

# Code coverage
#add_compile_options(--coverage -O0)

# Standard compiler warnings
add_library(project_warnings INTERFACE)

include(CompilerWarnings.cmake)
set_project_warnings(project_warnings)

option(ENABLE_TESTING "Enable Test Builds" ON)

add_subdirectory(src)
add_subdirectory(exercises)

if(ENABLE_TESTING)
  message("Building tests.")
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()


