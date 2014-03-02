cmake_minimum_required(VERSION 2.8)
project(Game)

# Set the external library folder
set(PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find OpenGL and include its headers
# find_package(OpenGL REQUIRED)

# Set the include folders
include_directories(${PROJECT_INCLUDE_DIR})
# include_directories(${OPENGL_INCLUDE_DIRS})

# Find GLFW amongst our external libraries
set(RENDERENGINE "RenderEngine")

set(GAME_SRCS 
    ${PROJECT_SOURCE_DIR}/test.cpp
)

add_executable(Game ${GAME_SRCS})
target_link_libraries(Game ${RENDERENGINE})

