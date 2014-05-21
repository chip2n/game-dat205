#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight : public Light {
    public:
        DirectionalLight() : Light(glm::vec3(0)) {};
        DirectionalLight(glm::vec3 position, glm::vec3 d)
          : Light(position), direction(d) {}
        glm::vec3 direction;
    private:
};

#endif

