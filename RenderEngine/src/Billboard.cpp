#include <GL/glew.h>
#include <iostream>
#include "Billboard.h"

GLfloat Billboard::plane[] = {
   // X       Y     Z     U     V     NX    NY   NZ
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

Billboard::Billboard(){
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 8*6*sizeof(float), plane, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (3*sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (5*sizeof(float)));

}

void Billboard::render(Camera &camera, Environment &env, ShaderProgram &shaderProgram) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    shaderProgram.begin();
    shaderProgram.setUniform("modelViewProjectionMatrix", camera.getCombinedMatrix());
    glm::mat4 modelM;
    modelM = glm::translate(modelM, position);
    modelM = glm::inverse(glm::lookAt(position, camera.position, glm::vec3(0.0f,1.0f,0.0f)));
    modelM = glm::scale(modelM, glm::vec3(size));
    shaderProgram.setUniform("modelMatrix", modelM);
    shaderProgram.setUniform("lightPos", env.getLights()[0].getPosition());
    shaderProgram.setUniform("receivesShadows", false);
    shaderProgram.setUniform("isLightSource", isLightSource);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    shaderProgram.end();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Billboard::move(glm::vec3 vec) {
    position = position + vec;
}
