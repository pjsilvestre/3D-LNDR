#include "particle.h"

void Particle::Integrate() {
  position_ += velocity_ * 1 / 60;
  acceleration_ += forces_;

  if (glm::length(velocity_) < constants::kParticleTerminalVelocity) {
    velocity_ += acceleration_ * 1 / 60;
  }

  velocity_ *= constants::kParticleVelocityDamping;
  acceleration_ *= constants::kParticleAccelerationDamping;

  forces_ = glm::vec3(0.0f);
}
