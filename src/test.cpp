#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>
#include "ShaderProgram.h"
#include "Camera.h"
#include "Room.h"
#include "OBJLoader.h"

Camera camera(45.0f, 640, 480);


void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

glm::vec3 movementDirection;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

    if(key == GLFW_KEY_W) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.y = 1;
        } else {
            movementDirection.y = 0;
        }
    }
    if(key == GLFW_KEY_S) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.y = -1;
        } else {
            movementDirection.y = 0;
        }
    }
    if(key == GLFW_KEY_A) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.x = -1;
        } else {
            movementDirection.x = 0;
        }
    }
    if(key == GLFW_KEY_D) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.x = 1;
        } else {
            movementDirection.x = 0;
        }
    }
}

double lastX, lastY;
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    double deltaX = xpos - lastX;
    double deltaY = ypos - lastY;

    camera.rotate(glm::vec3(0,1,0), -deltaX/10);
    camera.rotate(camera.getRight(), -deltaY/10);
    camera.update();

    lastX = xpos;
    lastY = ypos;
}

static void resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, const char *argv[]) {
    // Initiate GLFW or crash everything if failure, yo
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	if(!glfwInit()) {
		exit(EXIT_FAILURE);
    }

	 window = glfwCreateWindow(640, 480, "Title", NULL, NULL);

	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

    // Set up all them callbacks, yo
	glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, resize_callback);

	glfwMakeContextCurrent(window);

    // Disable the cursor, yo
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initiate GLEW or crash everything if failure, yo
    if(glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }

    // Load shaders, yo
    ShaderProgram shaderProgram("simpleshading.vert", "simpleshading.frag");

    // Init camera at position (2,3,3) looking at origin, yo
    camera.setPosition(glm::vec3(2,3,3));
    camera.lookAt(glm::vec3(0,0,0));
    camera.update();

    OBJLoader loader;
    std::vector<glm::vec3> obj;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    loader.loadOBJ("cube.obj", obj, texCoords, normals);

    // Load a texture using SOIL, yo
    GLuint tex = SOIL_load_OGL_texture ("bricks.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if(0 == tex) {
        std::cout << "Error loading texture." << std::endl;
    }


    // Create the VAO for all them shader sauces, yo
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate some of the buffers yo
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, obj.size()*sizeof(glm::vec3), &obj[0], GL_STATIC_DRAW);

    GLuint texCoordsBuffer;
    glGenBuffers(1, &texCoordsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordsBuffer);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size()*sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);

    GLuint normalsBuffer;
    glGenBuffers(1, &normalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // Let's create a light for great glory, yo
    glm::vec3 lightPos = glm::vec3(10, 4, 2);

    double lastTime = glfwGetTime();
    double deltaTime = lastTime;
	while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        camera.move(10*(float)deltaTime * (camera.getDirection() * movementDirection.y));
        camera.move(10*(float)deltaTime * (camera.getRight() * movementDirection.x));
        camera.update();

        glBindTexture(GL_TEXTURE_2D, tex);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0*sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordsBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0*sizeof(float), (void*)0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0*sizeof(float), (void*)0);

        shaderProgram.begin();
        shaderProgram.setUniform("modelViewProjectionMatrix", camera.getCombinedMatrix());
        glm::mat4 model;
        shaderProgram.setUniform("modelMatrix", model);
        shaderProgram.setUniform("lightPos", lightPos);
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        shaderProgram.end();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
