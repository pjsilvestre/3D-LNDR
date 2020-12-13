/**
 * @class LanderSystem
 * @brief Encapsulates a LanderSystem, which is a ParticleSystem to hold one
 * Lander and its associated ParticleForces
 * @author Patrick Silvestre
 */

#pragma once

#include "lander.h"
#include "particle-system.h"

class LanderSystem : public ParticleSystem {
 public:
  LanderSystem();

  void Draw() { lander_.Draw(); }
  void Update(const Octree& octree);

  // Particle setters, getters
  float get_orientation() const { return lander_.orientation_; }
  void set_position(const glm::vec3& position) { lander_.position_ = position; }
  glm::vec3 get_position() const { return lander_.position_; }

  // Lander setter, getters
  void enable_altimeter() { lander_.altimeter_enabled_ = true; }
  void disable_altimeter() { lander_.altimeter_enabled_ = false; }
  bool altimeter_enabled() const { return lander_.altimeter_enabled_; }
  bool is_loaded() const { return lander_.loaded_; }
  void select() { lander_.selected_ = true; }
  void unselect() { lander_.selected_ = false; }
  float get_altitude() const { return lander_.altitude_; }
  Box get_bounds() const { return lander_.bounds_; }

  void ForwardThrust();
  void LeftwardThrust();
  void BackwardThrust();
  void RightwardThrust();
  void UpwardThrust();
  void YawLeft();
  void YawRight();

 private:
  Lander lander_;
  GravityForce gravity_ = GravityForce(glm::vec3(0.0f, -0.01f, 0.0f));
  XZTurbulenceForce turbulence_ =
      XZTurbulenceForce(glm::vec3(-0.1f), glm::vec3(0.1f));
};
