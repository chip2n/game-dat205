#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <string>
#include "Mesh.h"

class GameObject {
    public:
        void setPosition(glm::vec3 pos);
        void setRotation(float angle);
        glm::vec3 getPosition();
        float getRotation();
        bool collectable;
        bool passable;
        std::string collectAnimation;
        glm::vec3 position;
        float rotation;
        Mesh *mesh;
    private:
};

#endif
