#ifndef MODELINSTANCE_H
#define MODELINSTANCE_H

#include "Model.h"
#include "Environment.h"

class ModelInstance {
    public:
        ModelInstance(Model *model);
        void render(Camera &camera, Environment &env, ShaderProgram &shaderProgram);
        void move(glm::vec3 vec);
        void rotate(float angle, glm::vec3 axis);
        glm::vec3 position;
        glm::mat4 rotation;
        Model *model;
        glm::mat4 getModelMatrix();
    private:
};

#endif
