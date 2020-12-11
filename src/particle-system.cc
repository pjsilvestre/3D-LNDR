#include "particle-system.h"

void ParticleSystem::Update() {
  if (particles_.empty()) {
    return;
  }

  Integrate();
  UpdateForces();
}

void ParticleSystem::AddForce(ParticleForce* particle_force) {
  if (particle_force == nullptr) {
    cerr << "ParticleSystem::AddForce() argument is null" << endl;
    return;
  }

  forces_.push_back(particle_force);
}

void ParticleSystem::AddParticle(Particle* particle) {
  if (particle == nullptr) {
    cerr << "ParticleSystem::AddParticle() argument is null" << endl;
    return;
  }

  particles_.push_back(particle);
}

void ParticleSystem::Integrate() {
  for (auto& particle : particles_) {
    particle->Integrate();
  }
}

void ParticleSystem::UpdateForces() {
  for (auto& particle : particles_) {
    for (auto& force : forces_) {
      force->Update(particle);
    }
  }
}
