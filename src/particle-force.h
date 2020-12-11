#pragma once

#include "particle.h"

class ParticleForce {
 public:
  virtual void Update(Particle* particle) = 0;
};

class GravityForce : public ParticleForce {
 public:
  GravityForce(const glm::vec3& gravity);

  void Update(Particle* particle) override;

 private:
  glm::vec3 gravity_ = glm::vec3(0.0f, -0.1f, 0.0f);
};

class TurbulenceForce : public ParticleForce {
 public:
  TurbulenceForce(const glm::vec3& min_turbulence,
                  const glm::vec3& max_turbulence);

  void Update(Particle* particle) override;

 protected:
  glm::vec3 min_turbulence_ = glm::vec3(-0.1);
  glm::vec3 max_turbulence_ = glm::vec3(0.1);
};

class XZTurbulenceForce : public TurbulenceForce {
 public:
  XZTurbulenceForce(const glm::vec3& min_turbulence,
                  const glm::vec3& max_turbulence);
  void Update(Particle* particle) override;
};
