#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

class GameObject {
    public:
        void setPosition(glm::vec3 pos);
        void setRotation(glm::vec3 rot);
        glm::vec3 getPosition();
        glm::vec3 getRotation();
    private:
        glm::vec3 position;
        glm::vec3 rotation;
};

#endif
