# Set the include folder
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(RenderEngine test.cpp ShaderProgram.cpp Camera.cpp)
target_link_libraries(RenderEngine ${GLFW} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})
