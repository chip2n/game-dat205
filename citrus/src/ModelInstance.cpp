#include <iostream>
#include <GL/glew.h>
#include "ModelInstance.h"

ModelInstance::ModelInstance(Model *m) {
    model = m;
}

void ModelInstance::render(Camera &camera, Environment &env, ShaderProgram &shaderProgram)  {
    glBindVertexArray(model->vao);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);

    shaderProgram.begin();
    shaderProgram.setUniform("modelViewProjectionMatrix", camera.getCombinedMatrix());
    glm::mat4 modelM = getModelMatrix();
    shaderProgram.setUniform("modelMatrix", modelM);
    shaderProgram.setUniform("lightPos", env.getLights()[0].getPosition());
    glDrawArrays(GL_TRIANGLES, 0, model->getNumberOfVertices());
    shaderProgram.end();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ModelInstance::move(glm::vec3 vec) {
    position = position + vec;
}

void ModelInstance::rotate(float angle, glm::vec3 axis) {
    rotation = glm::rotate(rotation, angle, axis);
}

glm::mat4 ModelInstance::getModelMatrix() {
    return glm::translate(glm::mat4(1.0), position) * rotation;
}
