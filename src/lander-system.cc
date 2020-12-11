#include "lander-system.h"

LanderSystem::LanderSystem() { particles_.push_back(lander_); }

void LanderSystem::Update(const Octree& octree) {
  lander_.Update(octree);
  // TODO add gravity
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