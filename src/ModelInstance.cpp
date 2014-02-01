#include <GL/glew.h>
#include "ModelInstance.h"

ModelInstance::ModelInstance(Model *m) {
    model = m;
}

void ModelInstance::render(Camera &camera, ShaderProgram &shaderProgram)  {
    glBindVertexArray(model->vao);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);

    shaderProgram.begin();
    shaderProgram.setUniform("modelViewProjectionMatrix", camera.getCombinedMatrix());
    glm::mat4 modelM;
    shaderProgram.setUniform("modelMatrix", modelM);
    glDrawArrays(GL_TRIANGLES, 0, model->positions.size()*3);
    shaderProgram.end();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
