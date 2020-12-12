/**
 * @class Particle
 * @brief Encapsulates an object whose position and motion is defined by applied
 * forces
 * @author Patrick Silvestre
 */

#pragma once

#include "constants.h"
#include "ofMain.h"

class Particle {
 public:
  void Integrate();

  float angular_acceleration_ = 0.0f;
  float angular_velocity_ = 0.0f;
  float lifespan_ = 0.0f;
  float orientation_ = 0.0f;  // degrees
  float rotational_forces_ = 0.0f;
  float spawn_time_ = 0.0f;

  glm::vec3 acceleration_ = glm::vec3(0.0f);
  glm::vec3 position_ = glm::vec3(0.0f);
  glm::vec3 positional_forces_ = glm::vec3(0.0f);
  glm::vec3 velocity_ = glm::vec3(0.0f);
  glm::mat4 transformation_matrix_ = glm::mat4(0.0f);

 private:
  void IntegratePosition();
  void IntegrateRotation();
};
