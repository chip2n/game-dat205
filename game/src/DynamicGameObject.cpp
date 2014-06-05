#include "DynamicGameObject.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <math.h>
#include <iostream>

void DynamicGameObject::move(glm::vec3 vec) {
    //setPosition(getPosition() + vec);
    currentMovement = vec;
}

void DynamicGameObject::rotate(float angle, glm::vec3 axis, float duration) {
    if(glm::angle(oldRotation) != angle) {
        rotationDuration = duration;
        rotationFinished = false;
        oldRotation = currentRotation;
        goalRotation = glm::angleAxis(angle, axis);
        rotStartTime = 0;
    }
}

void DynamicGameObject::update(float deltaTime) {
    rotStartTime += deltaTime;
    setPosition(getPosition() + currentMovement * deltaTime * speed);

    float factor = abs(glm::angle(goalRotation) - glm::angle(oldRotation)) / 90.0f;

    if(factor == 0.0f) factor = 1.0f;

    // Check if the rotation will take the long path - if so, negate goal rotation
    // to avoid this.
    if(glm::dot(oldRotation, goalRotation) < 0.0f) {
        goalRotation = -goalRotation;
    }

    if(rotStartTime/rotationDuration >= 1.0f) {
        rotationFinished = true;
        currentRotation = goalRotation;
    } else {
        currentRotation = glm::mix(oldRotation, goalRotation, rotStartTime / rotationDuration);
    }
}

void DynamicGameObject::stop() {
    goalRotation = currentRotation;
    currentMovement = glm::vec3(0,0,0);
    goalMovement = currentMovement;
}
