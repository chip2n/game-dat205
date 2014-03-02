#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class Window {
    public:
        Window();
        ~Window();

        GLFWwindow *window;
    private:
};

#endif
