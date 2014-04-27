#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include "DynamicGameObject.h"

#define MAX_HITPOINTS 100
#define START_X 0
#define START_Y 0
#define START_Z 0

class Player : public DynamicGameObject {
    public:
        Player() : DynamicGameObject(), hp(MAX_HITPOINTS) {}
    private:
        float hp;
};

#endif
