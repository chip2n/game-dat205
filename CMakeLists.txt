cmake_minimum_required(VERSION 2.8)

# Set the external library folder
set(LIBRARIES ${CMAKE_SOURCE_DIR}/libs)
# Set the include folder
include_directories(${CMAKE_SOURCE_DIR}/include)
# Set the source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Find OpenGL and include its headers
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

# Find GLFW amongst our external libraries
if(WIN32)
    find_library(GLEW_LIBRARY NAMES glew32.lib PATHS ${LIBRARIES})
    find_library(GLFW NAMES glfw3.lib PATHS ${LIBRARIES})
    find_library(SOIL NAMES libSOIL.lib PATHS ${LIBRARIES})
    # TODO: Add assimp
else(WIN32)
    find_package(GLEW REQUIRED)
    find_package(PkgConfig REQUIRED)
    set(GLFW "glfw")
    set(SOIL "SOIL")
    set(ASSIMP "assimp")
endif(WIN32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall")

add_subdirectory(${SOURCE_DIR})
