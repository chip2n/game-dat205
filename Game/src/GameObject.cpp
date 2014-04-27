#include "GameObject.h"

void GameObject::setPosition(glm::vec3 pos) {
    position = pos;
}

void GameObject::setRotation(glm::vec3 rot) {
    rotation = rot;
}

glm::vec3 GameObject::getPosition() {
    return position;
}

glm::vec3 GameObject::getRotation() {
    return rotation;
}
