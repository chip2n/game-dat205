#ifndef DYNAMIC_GAMEOBJECT_H
#define DYNAMIC_GAMEOBJECT_H

#include "GameObject.h"

class DynamicGameObject : public GameObject {
    public:
        DynamicGameObject() : GameObject() {}
        void move(glm::vec3 vec);
        virtual void update(float deltaTime);
    private:
        glm::vec3 currentMovement;
        glm::vec3 goalMovement;
        float smoothFactor;
};

#endif
