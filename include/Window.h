#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include "InputProcessor.h"

class Window {
    public:
        Window();
        ~Window();
        void setInputProcessor(InputProcessor inputProcessor);

        GLFWwindow *window;
    private:
};

#endif
