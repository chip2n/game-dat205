cmake_minimum_required(VERSION 2.8)
project(root)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall")

set(RENDERENGINE "citrus")
set(GAME "game")
set(TOOLS_DIR "tools")
set(EDITOR "citrus-editor")

add_subdirectory(${RENDERENGINE})
add_subdirectory(${GAME})
add_subdirectory(${TOOLS_DIR}/${EDITOR})
