#ifndef MODELINSTANCE_H
#define MODELINSTANCE_H

#include "Model.h"
#include "Environment.h"

class ModelInstance {
    public:
        ModelInstance(Model *model);
        void render(Camera &camera, Environment &env, ShaderProgram &shaderProgram);
        void move(glm::vec3 vec);
    private:
        Model *model;
        glm::vec3 position;
};

#endif