cmake_minimum_required(VERSION 2.8)
project(RenderEngine)

# Set the external library folder
set(PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find OpenGL and include its headers
find_package(OpenGL REQUIRED)

# Set the include folders
include_directories(${PROJECT_INCLUDE_DIR})
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

set(RENDERENGINE_SRCS 
    ${PROJECT_SOURCE_DIR}/ShaderProgram.cpp
    ${PROJECT_SOURCE_DIR}/Camera.cpp
    ${PROJECT_SOURCE_DIR}/Model.cpp
    ${PROJECT_SOURCE_DIR}/ModelInstance.cpp
    ${PROJECT_SOURCE_DIR}/Environment.cpp
    ${PROJECT_SOURCE_DIR}/Light.cpp
    ${PROJECT_SOURCE_DIR}/Billboard.cpp
    ${PROJECT_SOURCE_DIR}/Texture.cpp
    ${PROJECT_SOURCE_DIR}/ShadowMap.cpp
    ${PROJECT_SOURCE_DIR}/Window.cpp
    ${PROJECT_SOURCE_DIR}/InputProcessor.cpp
    ${PROJECT_SOURCE_DIR}/Screen.cpp
    ${PROJECT_SOURCE_DIR}/ScreenTest.cpp
)

add_library(${PROJECT_NAME} SHARED ${RENDERENGINE_SRCS})
target_link_libraries(${PROJECT_NAME} ${GLFW} ${SOIL} ${ASSIMP} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})
