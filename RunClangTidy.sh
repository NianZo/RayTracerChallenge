cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_CXX_COMPILER=clang-13
clang-tidy -p build src/*pp
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=0 -DCMAKE_CXX_COMPILER=g++