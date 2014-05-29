#include <cstdlib>
#include <algorithm>
#include <iostream>
#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {
  billboard.move(glm::vec3(0,1,-1));
  particles.push_back(Particle());
}

float ParticleEmitter::randomFloat() {
  return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

void ParticleEmitter::update(Camera &camera, Environment &env, ShaderProgram &shaderProgram, float deltaTime) {
  float spawnX = 2 * (randomFloat() - 0.5f);
  float spawnY = 2 * (randomFloat() - 0.5f);
  float spawnZ = 2 * (randomFloat() - 0.5f);
  float size = 0.3f * randomFloat();

  glm::vec3 spawnPosition = radius * glm::vec3(spawnX, spawnY, spawnZ) * 0.1f;

  lastSpawnTime += deltaTime;
  if(lastSpawnTime > 1/particlesPerSecond) {
    Particle p;
    p.position = spawnPosition;
    p.size = size;
    particles.push_back(p);
    lastSpawnTime = 0.0f;
  }
  std::vector<Particle>::iterator it = particles.begin();
  std::sort(&particles[0], &particles[particles.size() - 1]);
  while(it != particles.end()) {
    it->age += deltaTime;
    if(it->age > it->lifeSpan) {
      it = particles.erase(it);
    } else {
      it->position = it->position + velocity*deltaTime;
      it->cameradistance = glm::length(it->position - camera.position);
      billboard.position = position + it->position;
      billboard.size = it->size;
      billboard.render(camera, env, shaderProgram);
      ++it;
    }
  }
}
