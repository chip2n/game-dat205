#include "DynamicGameObject.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <math.h>

void DynamicGameObject::move(glm::vec3 vec) {
    //setPosition(getPosition() + vec);
    currentMovement = vec;
}

void DynamicGameObject::rotate(float angle) {
    oldRotation = getRotation();
    setRotation(rot);
}

void DynamicGameObject::update(float deltaTime) {
    setPosition(getPosition() + currentMovement * deltaTime * 10.0f);

    //float angle = glm::dot(oldRotation, rotation) / sqrt(oldRotation.x^2 + oldRotation.y^2 + oldRotation.z^2) * sqrt(rotation.x^2 + rotation.y^2;

    glm::quat q = glm::gtx::quaternion::angleAxis(getRotation(), up);
}
