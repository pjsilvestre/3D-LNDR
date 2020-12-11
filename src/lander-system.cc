#include "lander-system.h"

LanderSystem::LanderSystem() {
  particles_.push_back(&lander_);
  forces_.push_back(&gravity_);
  forces_.push_back(&turbulence_);
}

void LanderSystem::Update(const Octree& octree) {
  lander_.Update(octree);

  if (lander_.collision_boxes_.size() >= 250) {
    // assumption: perfectly elastic collision
    lander_.forces_ += -lander_.velocity_;
  }

  ParticleSystem::Update();
}

void LanderSystem::ForwardThrust() {
  lander_.forces_ +=
      glm::vec3(constants::kParticleInitialAcceleration, 0.0f, 0.0f);
}

void LanderSystem::LeftwardThrust() {
  lander_.forces_ +=
      glm::vec3(0.0f, 0.0f, -constants::kParticleInitialAcceleration);
}

void LanderSystem::BackwardThrust() {
  lander_.forces_ +=
      glm::vec3(-constants::kParticleInitialAcceleration, 0.0f, 0.0f);
}

void LanderSystem::RightwardThrust() {
  lander_.forces_ +=
      glm::vec3(0.0f, 0.0f, constants::kParticleInitialAcceleration);
}

void LanderSystem::UpwardThrust() {
  lander_.forces_ +=
      glm::vec3(0.0f, constants::kParticleInitialAcceleration, 0.0f);
}