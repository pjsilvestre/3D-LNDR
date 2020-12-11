#pragma once

#include "constants.h"
#include "lander.h"
#include "particle-system.h"

class LanderSystem : public ParticleSystem {
 public:
  LanderSystem();

  void Draw() { lander_.Draw(); }
  void Update(const Octree& octree);

  bool is_loaded() const { return lander_.loaded_; }
  glm::vec3 get_position() const { return lander_.position_; }
  void set_position(const glm::vec3& position) { lander_.position_ = position; }
  Box get_bounds() const { return lander_.bounds_; }
  void select() { lander_.selected_ = true; }
  void unselect() { lander_.selected_ = false; }

  void ForwardThrust();
  void LeftwardThrust();
  void BackwardThrust();
  void RightwardThrust();
  void UpwardThrust();

 private:
  Lander lander_;
};
