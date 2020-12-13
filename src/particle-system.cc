#include "particle-system.h"

void ParticleSystem::Update() {
  if (particles_.empty()) {
    return;
  }

  DeleteDeadParticles();
  Integrate();
  UpdateForces();
}

void ParticleSystem::Draw() const {
  for (const auto& particle : particles_) {
    particle->Draw();
  }
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

void ParticleSystem::DeleteDeadParticles() {
  auto iterator = particles_.begin();

  while (iterator != particles_.end()) {
    const Particle* particle = *iterator;

    if (particle->lifespan_ > 0.0f &&
        particle->GetAge() > particle->lifespan_) {
      // free allocated memory
      delete particle;
      particle = nullptr;

      // remove from vector
      iterator = particles_.erase(iterator);
    } else {
      ++iterator;
    }
  }
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
