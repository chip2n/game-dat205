#ifndef LEVEL_H
#define LEVEL_H

#include "Mesh.h"
#include "GameObject.h"
#include "Light.h"

class Level {
    public:
        Mesh *levelMesh;
        std::vector<Light> getLights();
        std::vector<Light> lights;
        std::vector<GameObject> gameObjects;
    private:
};

#endif
