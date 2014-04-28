#include "GameObject.h"

void GameObject::setPosition(glm::vec3 pos) {
    position = pos;
}

void GameObject::setRotation(float angle) {
    //deltaRot = rotation - rot;

    rotation = angle;
}

glm::vec3 GameObject::getPosition() {
    return position;
}

float GameObject::getRotation() {
    return rotation;
}
