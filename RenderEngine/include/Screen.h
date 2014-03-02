#ifndef SCREEN_H
#define SCREEN_H

#include "Camera.h"

class Screen {
    public:
        void setCamera(Camera *c);
        virtual void render();
        void clear();
        Camera *camera;
    private:
};

#endif
