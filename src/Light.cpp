#include "Light.h"

Light::Light(glm::vec3 p) {
    position = p;
}

glm::vec3 Light::getPosition() {
    return position;
}
