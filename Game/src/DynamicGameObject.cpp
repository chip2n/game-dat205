#include "DynamicGameObject.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <math.h>
#include <iostream>

void DynamicGameObject::move(glm::vec3 vec) {
    //setPosition(getPosition() + vec);
    currentMovement = vec;
}

void DynamicGameObject::rotate(float angle) {
    if(rotationFinished && oldRotation != angle) {
        rotationFinished = false;
        oldRotation = getRotation();
        setRotation(angle);
        rotStartTime = 0;
    }
}

void DynamicGameObject::update(float deltaTime) {
    rotStartTime += deltaTime;
    setPosition(getPosition() + currentMovement * deltaTime * speed);

    if(oldRotation >= 180.0f && getRotation() == 0.0f) {
        oldRotation = oldRotation - 360.0f;
    }

    if(oldRotation < 90.0f && getRotation() >= 180.0f) {
        setRotation(getRotation() - 360.0f);
    }

    glm::quat rotQuat = glm::angleAxis(getRotation(), up);
    glm::quat oldRotQuat = glm::angleAxis(oldRotation, up);

    float factor = abs(getRotation() - oldRotation) / 90.0f;

    if(factor == 0.0f) {
        factor = 1.0f;
    }


    if(turnSpeed*factor*rotStartTime >= 1.0f) {
        rotationFinished = true;
        currentRotation = rotQuat;
    } else {
        currentRotation = glm::mix(oldRotQuat, rotQuat, turnSpeed*factor*rotStartTime);
    }
}
