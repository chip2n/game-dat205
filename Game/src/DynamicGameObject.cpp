#include "DynamicGameObject.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <math.h>

void DynamicGameObject::move(glm::vec3 vec) {
    //setPosition(getPosition() + vec);
    currentMovement = vec;
}

void DynamicGameObject::rotate(float angle) {
    if(oldRotation != angle) {
        oldRotation = getRotation();
        setRotation(angle);
        rotStartTime = 0;
    }
}

void DynamicGameObject::update(float deltaTime) {
    rotStartTime += deltaTime;
    setPosition(getPosition() + currentMovement * deltaTime * 10.0f);

    //float angle = glm::dot(oldRotation, rotation) / sqrt(oldRotation.x^2 + oldRotation.y^2 + oldRotation.z^2) * sqrt(rotation.x^2 + rotation.y^2;

    glm::quat rotQuat = glm::angleAxis(getRotation(), up);
    glm::quat oldRotQuat = glm::angleAxis(oldRotation, up);

    currentRotation = glm::mix(oldRotQuat, rotQuat, rotStartTime);
    
}
