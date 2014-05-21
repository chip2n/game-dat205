#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <vector>
#include "Light.h"
#include "DirectionalLight.h"

class Environment {
    public:
        void addLight(Light light);
        std::vector<Light> getLights();
        DirectionalLight worldLight;
    private:
        std::vector<Light> lights;
};

#endif
