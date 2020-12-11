#pragma once

#include "particle-force.h"
#include "particle.h"

class ParticleSystem {
 public:
  void Update();

  void AddForce(ParticleForce* particle_force);
  void AddParticle(Particle* particle);

 protected:
  vector<Particle*> particles_;
  vector<ParticleForce*> forces_;

  void Integrate();
  void UpdateForces();
};
