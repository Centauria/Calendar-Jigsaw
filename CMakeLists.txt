cmake_minimum_required(VERSION 3.25)
project(AntPuzzle)

set(CMAKE_CXX_STANDARD 17)

add_executable(AntPuzzle main.cpp concept.cpp concept.h Vector2D.h Vector2D.cpp)
