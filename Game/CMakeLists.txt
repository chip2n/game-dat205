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
include_directories(${CMAKE_SOURCE_DIR}/RenderEngine/include)
# include_directories(${OPENGL_INCLUDE_DIRS})

set(RENDERENGINE "RenderEngine")

set(GAME_SRCS 
    ${PROJECT_SOURCE_DIR}/test2.cpp
    ${PROJECT_SOURCE_DIR}/Player.cpp
)

add_executable(${PROJECT_NAME} ${GAME_SRCS})
target_link_libraries(${PROJECT_NAME} ${RENDERENGINE})

# Copy assets
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
