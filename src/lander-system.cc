#include "lander-system.h"

LanderSystem::LanderSystem() {
  particles_.push_back(&lander_);
  forces_.push_back(&gravity_);
  forces_.push_back(&turbulence_);
}

void LanderSystem::Draw() { lander_.Draw(); }

void LanderSystem::Update(const Octree& octree) {
  lander_.Update(octree);

  if (lander_.collision_boxes_.size() > 10) {
    colliding_ = true;

    // assumption: relatively perfect elastic collision
    lander_.positional_forces_ += -lander_.velocity_;
  } else {
    colliding_ = false;
  }

  ParticleSystem::Update();
}

void LanderSystem::ForwardThrust() {
  lander_.positional_forces_ +=
      glm::rotateY(glm::vec3(lander_.initial_acceleration_, 0.0f, 0.0f),
                   glm::radians(lander_.orientation_));
}

void LanderSystem::LeftwardThrust() {
  lander_.positional_forces_ +=
      glm::rotateY(glm::vec3(0.0f, 0.0f, -lander_.initial_acceleration_),
                   glm::radians(lander_.orientation_));
}

void LanderSystem::BackwardThrust() {
  lander_.positional_forces_ +=
      glm::rotateY(glm::vec3(-lander_.initial_acceleration_, 0.0f, 0.0f),
                   glm::radians(lander_.orientation_));
}

void LanderSystem::RightwardThrust() {
  lander_.positional_forces_ +=
      glm::rotateY(glm::vec3(0.0f, 0.0f, lander_.initial_acceleration_),
                   glm::radians(lander_.orientation_));
}

void LanderSystem::UpwardThrust() {
  lander_.positional_forces_ +=
      glm::rotateY(glm::vec3(0.0f, lander_.initial_acceleration_, 0.0f),
                   glm::radians(lander_.orientation_));
}

void LanderSystem::YawLeft() {
  lander_.rotational_forces_ += lander_.initial_angular_acceleration_;
}

void LanderSystem::YawRight() {
  lander_.rotational_forces_ -= lander_.initial_angular_acceleration_;
}

void LanderSystem::Reset() {
  lander_.angular_acceleration_ = 0.0f;
  lander_.angular_velocity_ = 0.0f;
  lander_.orientation_ = 0.0f;
  lander_.rotational_forces_ = 0.0f;
  lander_.acceleration_ = glm::vec3(0.0f);
  lander_.position_ = glm::vec3(-45.0f, 65.0f, -45.0f);
  lander_.positional_forces_ = glm::vec3(0.0f);
  lander_.velocity_ = glm::vec3(0.0f);

  lander_.altimeter_enabled_ = false;
  lander_.selected_ = false;
}