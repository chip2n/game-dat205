#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <vector>
#include "Camera.h"
#include "ShaderProgram.h"
#include "Environment.h"
#include <glm/glm.hpp>
#include "Billboard.h"

struct Particle {
    float age = 0.0f;
    float lifeSpan = 0.3f;
    float size;
    glm::vec3 position;
    Billboard billboard;
    float cameradistance;
    bool operator < (Particle& that) {
      // Sort in reverse order : far particles drawn first.
      return this->cameradistance > that.cameradistance;
    }
};

class ParticleEmitter {
  public:
    ParticleEmitter();
    std::vector<Particle> particles;
    void update(Camera &camera, Environment &env, ShaderProgram &shaderProgram, float deltaTime);
    glm::vec3 velocity = glm::vec3(0,0.7,0);
    float radius = 0.7f;
    float particlesPerSecond = 100;
    glm::vec3 position;
    Billboard billboard;
  private:
    float lastSpawnTime;
    float randomFloat();

};

#endif
