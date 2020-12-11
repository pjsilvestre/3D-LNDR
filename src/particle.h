#pragma once

#include "constants.h"
#include "ofMain.h"

class Particle {
 public:
  void Integrate();

  float spawn_time_ = 0.0f;
  float lifespan_ = 0.0f;
  glm::vec3 position_ = glm::vec3(0.0f);
  glm::vec3 velocity_ = glm::vec3(0.0f);
  glm::vec3 acceleration_ = glm::vec3(0.0f);
  glm::vec3 forces_ = glm::vec3(0.0f);
  glm::mat4 transformation_matrix_ = glm::mat4(0.0f);
};
