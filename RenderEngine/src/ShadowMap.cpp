#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "ShadowMap.h"

#define SHADOWMAP_WIDTH 2048
#define SHADOWMAP_HEIGHT 2048

// http://www.altdevblogaday.com/2011/01/30/omni-directional-shadow-mapping/

ShadowMap::ShadowMap(ShaderProgram &program) : shaderProgram(program) {
    glGenFramebuffers(1, &shadowFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Failed to set up shadow map framebuffer." << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::begin() {
    double ViewPortParams[4];
    glGetDoublev(GL_VIEWPORT, ViewPortParams);
    prevWidth = ViewPortParams[2];
    prevHeight = ViewPortParams[3];

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);
    glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
    glDrawBuffer(GL_NONE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.begin();
}

void ShadowMap::end() {
    shaderProgram.end();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glViewport(0, 0, prevWidth, prevHeight);
}

void ShadowMap::render(Mesh &mesh, glm::vec3 position, glm::quat rotation) {
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20,20,-20,20,-100,20);
    glm::mat4 depthViewMatrix = glm::lookAt(sunPosition, sunFocus, glm::vec3(0,1,0));
    //glm::mat4 depthModelMatrix = instance.getModelMatrix();
    glm::mat4 depthModelMatrix;
    depthModelMatrix = glm::translate(depthModelMatrix, position) * glm::toMat4(rotation);
    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
    shaderProgram.setUniform("depthMVP", depthMVP);
    shaderProgram.setUniform("hasBones", mesh.currentTransforms.size() > 0);
    mesh.render(shaderProgram);
    shaderProgram.begin();
}

glm::mat4 ShadowMap::getBiasMVP(glm::vec3 position) {
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20,20,-20,20,-100,20);
    glm::mat4 depthViewMatrix = glm::lookAt(sunPosition, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    depthModelMatrix = glm::translate(depthModelMatrix, position);
    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

    glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
    );

    return biasMatrix * depthMVP;
}

glm::mat4 ShadowMap::getBiasMVP(glm::vec3 position, glm::quat rotation) {
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20,20,-20,20,-100,20);
    glm::mat4 depthViewMatrix = glm::lookAt(sunPosition, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    depthModelMatrix = glm::translate(depthModelMatrix, position);
    depthModelMatrix = depthModelMatrix * glm::toMat4(rotation);
    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

    glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
    );

    return biasMatrix * depthMVP;
}
