cmake_minimum_required(VERSION 2.8)
project(game)

find_package(GLUT)
find_package(OpenGL)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(test test.cpp)
