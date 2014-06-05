#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include "Mesh.h"
#include "GameObject.h"
#include "Level.h"

class ResourceManager {
    public:
        Mesh* loadMesh(std::string path);
        Level* loadLevel(std::string path);
        GameObject loadGameObject(std::string path);
        std::map<std::string, Mesh*> meshes;
        std::map<std::string, Level> levels;
    private:
};

#endif
