# RayTracerChallenge
Test driven code following along with 'The Ray Tracer Challenge' book by James Buck

## Build notes for Eclipse:
Interfacing CMake projects with Eclipse seems to be a bit touchy. First, clone the repository. Then, create an empty CMake project in Eclipse and point it at the directory where you cloned this project. Otherwise, Eclipse's build tools will not work nicely with CMake. CMake build tools should still work fine from the command line, though.

## Testing Framework
GTest is used for unit tests in this project. Ensure you have gtest installed or tests won't compile and run.

## To Get Started:
After pulling this repository, set up CMake by running 'cmake .' in the project root directory. Then, 'cmake --build . --target all' can be run to build the project.
