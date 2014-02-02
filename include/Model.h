#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Camera.h"
#include "ShaderProgram.h"

class Model {
    public:
        bool loadFromFile(std::string path);
        unsigned int getNumberOfVertices();
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<float> test;
        GLuint vao;
        GLuint vbo;
    private:

};

#endif
