#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Camera.h"
#include "ShaderProgram.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model {
    public:
        Assimp::Importer importer;
        bool loadFromFile(std::string path);
        virtual void setupBuffers();
        unsigned int getNumberOfVertices();
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<float> test;
        GLuint vao;
        GLuint vbo;
        const aiScene* scene;
    private:

};

#endif
