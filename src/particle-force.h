#pragma once

#include "particle.h"

class ParticleForce {
 public:
  virtual void Update(Particle& particle) = 0;
};

class GravityForce : public ParticleForce {
 public:
  GravityForce(const glm::vec3& gravity);

  void Update(Particle& particle) override;

 private:
  glm::vec3 gravity_ = glm::vec3(0.0f, -10.0f, 0.0f);
};
