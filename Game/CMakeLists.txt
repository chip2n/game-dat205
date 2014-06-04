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
    ${PROJECT_SOURCE_DIR}/Main.cpp
    ${PROJECT_SOURCE_DIR}/Player.cpp
    ${PROJECT_SOURCE_DIR}/GameObject.cpp
    ${PROJECT_SOURCE_DIR}/DynamicGameObject.cpp
    ${PROJECT_SOURCE_DIR}/Level.cpp
    ${PROJECT_SOURCE_DIR}/ResourceManager.cpp
)

add_executable(${PROJECT_NAME} ${GAME_SRCS})
target_link_libraries(${PROJECT_NAME} ${RENDERENGINE})

add_custom_command(
  OUTPUT assets_synced
  COMMAND rsync -r "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${PROJECT_BINARY_DIR}"
  COMMENT "Syncing assets... ${PROJECT_BINARY_DIR}>")

add_custom_target(
  Assets ALL
  DEPENDS assets_synced
)
