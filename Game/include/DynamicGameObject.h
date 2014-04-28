#ifndef DYNAMIC_GAMEOBJECT_H
#define DYNAMIC_GAMEOBJECT_H

#include "GameObject.h"

class DynamicGameObject : public GameObject {
    public:
        DynamicGameObject() : GameObject() {}
        void move(glm::vec3 vec);
        void rotate(float angle);
        virtual void update(float deltaTime);
    private:
        glm::vec3 currentMovement;
        glm::vec3 goalMovement;
        float oldRotation;
        glm::vec3 up = glm::vec3(0,1,0);
        float smoothFactor;
};

#endif
