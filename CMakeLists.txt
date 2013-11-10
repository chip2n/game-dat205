cmake_minimum_required(VERSION 2.8)
project(game)

find_package(OpenGL)

add_subdirectory(lib/glfw-3.0.3)
include_directories(lib/glfw-3.0.3/include)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(test test.cpp)
target_link_libraries(test glfw ${GLFW_LIBRARIES})
