#ifndef LEVEL_H
#define LEVEL_H

#include "Mesh.h"
#include "GameObject.h"
#include "Light.h"
#include "Environment.h"
#include "ParticleEmitter.h"

class Level {
    public:
        Mesh *levelMesh;
        //std::vector<Light> getLights();
        Environment environment;
        std::vector<Light> lights;
        std::vector<GameObject> gameObjects;
        std::vector<ParticleEmitter> emitters;
    private:
};

#endif
