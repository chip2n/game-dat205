cmake_minimum_required(VERSION 2.8)
project(root)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall")

add_subdirectory(RenderEngine)
add_subdirectory(Game)
