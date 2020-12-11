#include "particle.h"

void Particle::Integrate() {
  IntegratePosition();
  IntegrateRotation();
}

void Particle::IntegratePosition() {
  position_ += velocity_ * 1 / 60;
  acceleration_ += positional_forces_;

  if (glm::length(velocity_) < constants::kParticleTerminalVelocity) {
    velocity_ += acceleration_ * 1 / 60;
  }

  velocity_ *= constants::kParticleVelocityDamping;
  acceleration_ *= constants::kParticleAccelerationDamping;

  positional_forces_ = glm::vec3(0.0f);
}

void Particle::IntegrateRotation() {
  orientation_ += angular_velocity_ * 1 / 60;
  angular_acceleration_ += rotational_forces_;

  if (abs(angular_velocity_) < constants::kParticleTerminalAngularVelocity) {
    angular_velocity_ += angular_acceleration_ * 1 / 60;
  }

  angular_velocity_ *= constants::kParticleAngularVelocityDamping;
  angular_acceleration_ *= constants::kParticleAngularAccelerationDamping;

  rotational_forces_ = 0.0f;
}
