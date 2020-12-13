/**
 * @class ParticleSystem
 * @brief Encapsulates a collection of Particles and the ParticleForces applied
 * on them
 * @author Patrick Silvestre
 */

#pragma once

#include "particle-force.h"
#include "particle.h"

class ParticleSystem {
 public:
  void Update();
  void Draw() const;

  void AddForce(ParticleForce* particle_force);
  void AddParticle(Particle* particle);

  vector<Particle*> particles_;
  vector<ParticleForce*> forces_;

 private:
  void DeleteDeadParticles();
  void Integrate();
  void UpdateForces();
};
