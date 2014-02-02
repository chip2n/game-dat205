#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Environment.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "Texture.h"

class Billboard {
    public:
        Billboard(Texture &tex);
        void render(Camera &camera, Environment &env, ShaderProgram &shaderProgram);
    private:
        GLuint vao;
        GLuint vbo;
        static GLfloat plane[];
        glm::vec3 position;
};

#endif
