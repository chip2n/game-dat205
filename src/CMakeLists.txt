cmake_minimum_required(VERSION 2.8)
project(test)

add_executable(test ${SOURCE_DIR}/test.cpp ${SOURCE_DIR}/ShaderProgram.cpp)
target_link_libraries(test ${GLFW} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})
