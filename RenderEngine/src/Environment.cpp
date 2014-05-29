#include <algorithm>
#include "Environment.h"

void Environment::addLight(Light light) {
    lights.push_back(light);
}

std::vector<Light> Environment::getClosestLights(glm::vec3 pos) {
    return getClosestLights(pos, 4);
}

std::vector<Light> Environment::getClosestLights(glm::vec3 pos, int lightCap) {
    std::vector<Light> closestLights;
    currentComparePosition = pos;

    std::sort(lights.begin(), lights.end(), Less(*this));

    for(int i = 0; i < lightCap; i++) {
        closestLights.push_back(lights[i]);
    }

    return closestLights;
}

std::vector<Light> Environment::getLights() {
    return lights;
}

