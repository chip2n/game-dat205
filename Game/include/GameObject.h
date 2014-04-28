#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

class GameObject {
    public:
        void setPosition(glm::vec3 pos);
        void setRotation(float angle);
        glm::vec3 getPosition();
        float getRotation();
    private:
        glm::vec3 position;
        float rotation;
};

#endif
