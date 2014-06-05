#ifndef DYNAMIC_GAMEOBJECT_H
#define DYNAMIC_GAMEOBJECT_H

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include "GameObject.h"

class DynamicGameObject : public GameObject {
    public:
        DynamicGameObject() : GameObject() {}
        void move(glm::vec3 vec);
        void rotate(float angle, glm::vec3 axis, float duration);
        virtual void update(float deltaTime);
        bool rotationFinished = false;
        glm::vec3 rotAxis = glm::vec3(0,1,0);
        glm::vec3 oldRotAxis = glm::vec3(0,1,0);
        float turnSpeed = 5.0f;
        float speed = 2.5f;
        glm::quat oldRotation;
        glm::quat currentRotation;
        glm::quat goalRotation;
        float rotationDuration = 1.0f;
        void stop();
        glm::vec3 up = glm::vec3(0,1,0);
    private:
        float rotStartTime;
        glm::vec3 currentMovement;
        glm::vec3 goalMovement;
        float smoothFactor;
};

#endif
