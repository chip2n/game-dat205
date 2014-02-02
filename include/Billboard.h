#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Environment.h"
#include "Camera.h"
#include "ShaderProgram.h"

class Billboard {
    public:
        Billboard();
        void render(Camera &camera, Environment &env, ShaderProgram &shaderProgram);
    private:
        GLuint vao;
        GLuint vbo;
        static GLfloat plane[];
        glm::vec3 position;
};

#endif
