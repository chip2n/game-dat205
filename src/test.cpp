#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Camera.h"

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
            std::cout << "release" << std::endl;
            movementDirection.y = 0;
        }
    }
    if(key == GLFW_KEY_S) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.y = -1;
        } else {
            std::cout << "release" << std::endl;
            movementDirection.y = 0;
        }
    }
    if(key == GLFW_KEY_A) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.x = -1;
        } else {
            std::cout << "release" << std::endl;
            movementDirection.x = 0;
        }
    }
    if(key == GLFW_KEY_D) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.x = 1;
        } else {
            std::cout << "release" << std::endl;
            movementDirection.x = 0;
        }
    }
}

double lastX, lastY;
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    double deltaX = xpos - lastX;
    double deltaY = ypos - lastY;

    std::cout << "x: " << deltaX << ", y: " << deltaY << std::endl;
    camera.rotate(glm::vec3(0,1,0), -deltaX/10);
    camera.rotate(camera.getRight(), -deltaY/10);
    camera.update();

    lastX = xpos;
    lastY = ypos;
}

int main(int argc, const char *argv[]) {
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	if(!glfwInit())
		exit(EXIT_FAILURE);

	 window = glfwCreateWindow(640, 480, "Title", NULL, NULL);

	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
	glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }

    // Load shader
    ShaderProgram shaderProgram("simple.vert", "simple.frag");

    // Init camera at position (2,3,3) looking at origin
    camera.setPosition(glm::vec3(2,3,3));
    camera.lookAt(glm::vec3(0,0,0));
    camera.update();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    /* TEST RENDER TRIANGLE */
    const GLfloat triangle[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    double lastTime = glfwGetTime();
    double deltaTime = lastTime;

	while(!glfwWindowShouldClose(window)) {
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        camera.move(10*(float)deltaTime * (camera.getDirection() * movementDirection.y));
        camera.move(10*(float)deltaTime * (camera.getRight() * movementDirection.x));
        camera.update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        shaderProgram.begin();
        shaderProgram.setUniform("modelViewProjectionMatrix", camera.getCombinedMatrix());
        glDrawArrays(GL_TRIANGLES, 0, 3);
        shaderProgram.end();

        glDisableVertexAttribArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

