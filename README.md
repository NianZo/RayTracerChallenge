[![Actions Status](https://github.com/NianZo/RayTracerChallenge/workflows/Build%20and%20Test/badge.svg)](https://github.com/NianZo/RayTracerChallenge/actions)
[![Actions Status](https://github.com/NianZo/RayTracerChallenge/workflows/Clang%20Format/badge.svg)](https://github.com/NianZo/RayTracerChallenge/actions)
[![Actions Status](https://github.com/NianZo/RayTracerChallenge/workflows/Clang%20Tidy/badge.svg)](https://github.com/NianZo/RayTracerChallenge/actions)
[![codecov](https://codecov.io/gh/NianZo/RayTracerChallenge/branch/main/graph/badge.svg?token=PC1EF3KWN5)](https://codecov.io/gh/NianZo/RayTracerChallenge)
# RayTracerChallenge
Test driven code following along with 'The Ray Tracer Challenge' book by Jamis Buck
![Rendered in 6760.78 seconds](https://user-images.githubusercontent.com/24691990/156849784-da2575d8-1062-4c8a-8ccf-cdbdf6286908.png)

## Building this project
This project utilizes CMake. After cloning the repository run the following commands from the project root directory to build and then run the unit tests:
```
cmake -B build -S .
make -C build
ctest --test-dir build
```
In addition, the main project executable will be in build/src.

## Build notes for Eclipse:
Interfacing CMake projects with Eclipse seems to be a bit touchy. First, clone the repository. Then, create an empty CMake project in Eclipse and point it at the directory where you cloned this project. Otherwise, Eclipse's build tools will not work nicely with CMake. CMake build tools should still work fine from the command line, though.

## Testing Framework
GTest is used for unit tests in this project. Ensure you have gtest installed or tests won't compile and run.

## Notes
ClangTidy is currently failing in CI. GitHub Actions isn't able to install the proper version of clang (clang-13) for some reason
