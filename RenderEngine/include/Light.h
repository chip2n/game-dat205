#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {
    public:
        Light(glm::vec3 position);
        Light(const Light &light);
        glm::vec3 getPosition();
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
    private:
};

#endif
