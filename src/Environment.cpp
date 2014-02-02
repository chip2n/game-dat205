#include "Environment.h"

void Environment::addLight(Light light) {
    lights.push_back(light);
}

std::vector<Light> Environment::getLights() {
    return lights;
}
