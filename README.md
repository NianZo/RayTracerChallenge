[![Actions Status](https://github.com/NianZo/RayTracerChallenge/workflows/Build%20and%20Test/badge.svg)](https://github.com/NianZo/RayTracerChallenge/actions)
# RayTracerChallenge
Test driven code following along with 'The Ray Tracer Challenge' book by Jamis Buck

## Building this project
This project utilizes CMake. After cloning the repository run the following commands from the project root directory to build and then run the unit tests:
cmake -B build -S .
make -C build
ctest --test-dir build
In addition, the main project executable will be in build/src.

## Build notes for Eclipse:
Interfacing CMake projects with Eclipse seems to be a bit touchy. First, clone the repository. Then, create an empty CMake project in Eclipse and point it at the directory where you cloned this project. Otherwise, Eclipse's build tools will not work nicely with CMake. CMake build tools should still work fine from the command line, though.

## Testing Framework
GTest is used for unit tests in this project. Ensure you have gtest installed or tests won't compile and run.

## To Get Started:
After pulling this repository, set up CMake by running 'cmake .' in the project root directory. Then, 'cmake --build . --target all' can be run to build the project.
