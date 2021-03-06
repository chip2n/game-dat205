#ifndef SHADOWMAP_H
#define SHADOWNAP_H

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ShaderProgram.h"
#include "Mesh.h"

class ShadowMap {
    public:
        ShadowMap(ShaderProgram &program);
        void begin();
        void end();
        void render(Mesh &mesh, glm::vec3 position, glm::quat rotation);
        glm::mat4 getBiasMVP(glm::vec3 position);
        glm::mat4 getBiasMVP(glm::vec3 position, glm::quat rotation);
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
