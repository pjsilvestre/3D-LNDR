/**
 * @class Particle
 * @brief Encapsulates an object whose position and motion is defined by applied
 * forces
 * @author Patrick Silvestre
 */

#pragma once

#include "ofMain.h"

class Particle {
 public:
  void Update();
  virtual void Draw() const;

  void Integrate();
  float GetAge() const;

  float acceleration_damping_ = 0.99f;
  float angular_acceleration_ = 0.0f;
  float angular_acceleration_damping_ = 0.99f;
  float angular_velocity_ = 0.0f;
  float angular_velocity_damping_ = 0.99f;
  float initial_acceleration_ = 1.0f;
  float initial_angular_acceleration_ = 10.0f;
  float lifespan_ = 0.5f;
  float orientation_ = 0.0f;  // degrees
  float radius_ = 0.1f;
  float rotational_forces_ = 0.0f;
  float spawn_time_ = ofGetElapsedTimef();
  float terminal_angular_velocity_ = 15.0f;
  float terminal_velocity_ = 5.0f;
  float velocity_damping_ = 0.99f;

  glm::vec3 acceleration_ = glm::vec3(0.0f);
  glm::vec3 position_ = glm::vec3(0.0f);
  glm::vec3 positional_forces_ = glm::vec3(0.0f);
  glm::vec3 velocity_ = glm::vec3(0.0f);
  glm::mat4 transformation_matrix_ = glm::mat4(0.0f);

  ofColor color_ = ofColor::red;

 private:
  void IntegratePosition();
  void IntegrateRotation();
};
