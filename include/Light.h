#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {
    public:
        Light(glm::vec3 position);
        glm::vec3 getPosition();
    private:
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
};

#endif
