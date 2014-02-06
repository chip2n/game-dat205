#ifndef SHADOWMAP_H
#define SHADOWNAP_H

#include "ShaderProgram.h"
#include "ModelInstance.h"

class ShadowMap {
    public:
        ShadowMap(ShaderProgram &program);
        void begin();
        void render(ModelInstance &instance);
        void end();
        glm::mat4 getBiasMVP(glm::vec3 position);
        ShaderProgram &shaderProgram;
        GLuint shadowFramebuffer;
        GLuint depthTexture;
    private:
};

#endif
