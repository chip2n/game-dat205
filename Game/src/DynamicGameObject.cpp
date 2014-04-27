#include "DynamicGameObject.h"

void DynamicGameObject::move(glm::vec3 vec) {
    //setPosition(getPosition() + vec);
    currentMovement = vec;
}

void DynamicGameObject::update(float deltaTime) {
    setPosition(getPosition() + currentMovement * deltaTime * 10.0f);
}
