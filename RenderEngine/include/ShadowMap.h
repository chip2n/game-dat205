#ifndef SHADOWMAP_H
#define SHADOWNAP_H

#include "ShaderProgram.h"
#include "Mesh.h"

class ShadowMap {
    public:
        ShadowMap(ShaderProgram &program);
        void begin();
        void end();
        void render(Mesh &mesh, glm::vec3 position);
        glm::mat4 getBiasMVP(glm::vec3 position);
        ShaderProgram &shaderProgram;
        GLuint shadowFramebuffer;
        GLuint depthTexture;
        glm::vec3 sunPosition;
        glm::vec3 sunFocus;
    private:
        int prevWidth;
        int prevHeight;
};

#endif
