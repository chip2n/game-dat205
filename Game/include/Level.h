#ifndef LEVEL_H
#define LEVEL_H

#include "Mesh.h"
#include "GameObject.h"

class Level {
    public:
        Mesh *levelMesh;
        std::vector<GameObject> gameObjects;
    private:
};

#endif
