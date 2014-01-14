#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <vector>

class OBJLoader {
    public:
        std::vector<glm::vec3> loadOBJ(std::string path);
    private:
        glm::vec3 readVertex(std::string line);
};

#endif
