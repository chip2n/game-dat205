#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <vector>

class OBJLoader {
    public:
        void loadOBJ(
                std::string path,
                std::vector<glm::vec3> &vertices,
                std::vector<glm::vec2> &texCoords,
                std::vector<glm::vec3> &normals
        );
    private:
        glm::vec3 readVertex(std::string line);
        glm::vec2 readTexCoord(std::string line);
};

#endif
