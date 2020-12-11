#include "particle-system.h"

void ParticleSystem::Update() {
  if (particles_.empty()) {
    return;
  }

  UpdateForces();
  Integrate();
}

void ParticleSystem::AddForce(ParticleForce* particle_force) {
  forces_.push_back(particle_force);
}

void ParticleSystem::AddParticle(const Particle& particle) {
  particles_.push_back(particle);
}

void ParticleSystem::Integrate() {
  for (auto& particle : particles_) {
    particle.Integrate();
  }
}

void ParticleSystem::UpdateForces() {
  for (auto& particle : particles_) {
    for (auto& force : forces_) {
      force->Update(particle);
    }
  }
}
