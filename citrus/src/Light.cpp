#include "Light.h"

Light::Light(glm::vec3 p) {
    position = p;
}

Light::Light(const Light &light) {
    position = light.position;
    color = light.color;
    intensity = light.intensity;
}

glm::vec3 Light::getPosition() {
    return position;
}
