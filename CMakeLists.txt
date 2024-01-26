cmake_minimum_required(VERSION 3.26)
project(keleidoscope)

set(CMAKE_CXX_STANDARD 17)

add_executable(keleidoscope main.cpp
        ast.cpp
        parser.cpp
        tokens.cpp
        optimizations.cpp)
