#ifndef MODELINSTANCE_H
#define MODELINSTANCE_H

#include "Model.h"

class ModelInstance {
    public:
        ModelInstance(Model *model);
        void render(Camera &camera, ShaderProgram &shaderProgram);
    private:
        Model *model;

};

#endif
