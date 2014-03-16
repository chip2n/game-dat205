#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

typedef void(* KeyPressCallback)(int, int);

class Window {
    public:
        Window();
        ~Window();
        void makeCurrent();
        static void setKeyCallback(KeyPressCallback);

        GLFWwindow *window;

        static KeyPressCallback callback;
    private:
};

#endif
