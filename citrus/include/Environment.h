#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <vector>
#include "Light.h"
#include "DirectionalLight.h"

class Environment {
    public:
        void addLight(Light light);
        std::vector<Light> getLights();
        std::vector<Light> getClosestLights(glm::vec3 pos);
        std::vector<Light> getClosestLights(glm::vec3 pos, int lightCap);
        struct Less {
            Less(const Environment& e) : env(e) {}
            bool operator () ( const Light & l1, const Light & l2 ) {
                float d1 = glm::length(l1.position - env.currentComparePosition);
                float d2 = glm::length(l2.position - env.currentComparePosition);

                return d1<d2;
            };
            const Environment& env;
        };
        glm::vec3 currentComparePosition;
    private:
        std::vector<Light> lights;
};

#endif
