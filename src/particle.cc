#include "particle.h"

float Particle::GetAge() const { return ofGetElapsedTimef() - spawn_time_; }

void Particle::Integrate() {
  IntegratePosition();
  IntegrateRotation();
}

void Particle::IntegratePosition() {
  position_ += velocity_ * 1 / 60;
  acceleration_ += positional_forces_;

  if (glm::length(velocity_) < terminal_velocity_) {
    velocity_ += acceleration_ * 1 / 60;
  }

  velocity_ *= velocity_damping_;
  acceleration_ *= acceleration_damping_;

  positional_forces_ = glm::vec3(0.0f);
}

void Particle::IntegrateRotation() {
  orientation_ += angular_velocity_ * 1 / 60;
  angular_acceleration_ += rotational_forces_;

  if (abs(angular_velocity_) < terminal_angular_velocity_) {
    angular_velocity_ += angular_acceleration_ * 1 / 60;
  }

  angular_velocity_ *= angular_velocity_damping_;
  angular_acceleration_ *= angular_acceleration_damping_;

  rotational_forces_ = 0.0f;
}
