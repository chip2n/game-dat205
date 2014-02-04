#ifndef MODELINSTANCE_H
#define MODELINSTANCE_H

#include "Model.h"
#include "Environment.h"

class ModelInstance {
    public:
        ModelInstance(Model *model);
        void render(Camera &camera, Environment &env, ShaderProgram &shaderProgram);
        void move(glm::vec3 vec);
        glm::vec3 position;
    private:
        Model *model;
};

#endif
