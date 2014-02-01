# Set the include folder
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(RenderEngine test.cpp ShaderProgram.cpp Camera.cpp OBJLoader.cpp Model.cpp ModelInstance.cpp)
target_link_libraries(RenderEngine ${GLFW} ${SOIL} ${ASSIMP} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})


add_custom_command(TARGET RenderEngine PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:RenderEngine>)
