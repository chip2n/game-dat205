#include "Window.h"
#include "WindowException.h"
#include <cstdio>
#include <iostream>

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

Window::Window() {
	glfwSetErrorCallback(error_callback);

	if(!glfwInit()) {
        throw new WindowException("Failed to initialize GLFW.");
    } else {
        window = glfwCreateWindow(640, 480, "Title", NULL, NULL);
        if(!window) {
            glfwTerminate();
            throw new WindowException("Could not create window.");
        }
        glfwMakeContextCurrent(window);

        // Disable the cursor, yo
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
