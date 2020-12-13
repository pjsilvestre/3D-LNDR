/**
 * @class ParticleEmitter
 * @brief Encapsulates an object that emits Particles with some initial velocity
 * @author Patrick Silvestre
 */

#pragma once

#include "particle-system.h"

class ParticleEmitter {
 public:
  void Draw() const;
  void Update();

  void Start();
  void Stop();

  bool fired_ = false;
  bool one_shot_ = false;
  bool started_ = false;
  int group_size_ = 10000;
  int rate_ = 1000;
  float last_spawn_time_ = -1.0f;
  glm::vec3 initial_velocity_ = glm::vec3(0.0f);
  glm::vec3 position_ = glm::vec3(0.0f);
  ParticleSystem particle_system_;

 private:
  virtual void Spawn(float spawn_time);
};

class ThrustParticleEmitter : public ParticleEmitter {
 public:
  ThrustParticleEmitter();

 private:
  void Spawn(float spawn_time) override;
};
