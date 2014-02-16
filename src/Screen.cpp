#include <GL/glew.h>
#include "Screen.h"

void Screen::setCamera(Camera *c) {
    camera = c;
}

void Screen::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
