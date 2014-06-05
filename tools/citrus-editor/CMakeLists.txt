cmake_minimum_required(VERSION 2.8)

project(${EDITOR})

# Set the external library folder
set(PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find OpenGL and include its headers
# find_package(OpenGL REQUIRED)

# Set the include folders
include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/${RENDERENGINE}/include)
# include_directories(${OPENGL_INCLUDE_DIRS})

set(EDITOR_SRCS 
    ${PROJECT_SOURCE_DIR}/Main.cpp
)

add_executable(${PROJECT_NAME} ${EDITOR_SRCS})
target_link_libraries(${PROJECT_NAME} ${RENDERENGINE})
