#include "lander-system.h"

LanderSystem::LanderSystem() {
  particles_.push_back(&lander_);
  forces_.push_back(&gravity_);
  forces_.push_back(&turbulence_);
}

void LanderSystem::Update(const Octree& octree) {
  lander_.Update(octree);

  if (lander_.collision_boxes_.size() >= 250) {
    // assumption: relatively perfect elastic collision
    lander_.positional_forces_ += -lander_.velocity_;
  }

  ParticleSystem::Update();
}

void LanderSystem::ForwardThrust() {
  lander_.positional_forces_ += glm::rotateY(
      glm::vec3(lander_.initial_acceleration_, 0.0f, 0.0f),
      glm::radians(lander_.orientation_));
}

void LanderSystem::LeftwardThrust() {
  lander_.positional_forces_ += glm::rotateY(
      glm::vec3(0.0f, 0.0f, -lander_.initial_acceleration_),
      glm::radians(lander_.orientation_));
}

void LanderSystem::BackwardThrust() {
  lander_.positional_forces_ += glm::rotateY(
      glm::vec3(-lander_.initial_acceleration_, 0.0f, 0.0f),
      glm::radians(lander_.orientation_));
}

void LanderSystem::RightwardThrust() {
  lander_.positional_forces_ += glm::rotateY(
      glm::vec3(0.0f, 0.0f, lander_.initial_acceleration_),
      glm::radians(lander_.orientation_));
}

void LanderSystem::UpwardThrust() {
  lander_.positional_forces_ += glm::rotateY(
      glm::vec3(0.0f, lander_.initial_acceleration_, 0.0f),
      glm::radians(lander_.orientation_));
}

void LanderSystem::YawLeft() {
  lander_.rotational_forces_ += lander_.initial_angular_acceleration_;
}

void LanderSystem::YawRight() {
  lander_.rotational_forces_ -= lander_.initial_angular_acceleration_;
}