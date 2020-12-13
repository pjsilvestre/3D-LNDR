#include "particle-emitter.h"

void ParticleEmitter::Draw() const { particle_system_.Draw(); }

void ParticleEmitter::Update() {
  if (started_) {
    const float current_time = ofGetElapsedTimeMillis();

    if ((current_time - last_spawn_time_) > (1000.0f / rate_)) {
      for (auto i = 0; i < group_size_; i++) {
        Spawn(current_time);
      }

      last_spawn_time_ = current_time;
    }
  }

  particle_system_.Update();
}

void ParticleEmitter::Start() {
  started_ = true;
  last_spawn_time_ = ofGetElapsedTimeMillis();
}

void ParticleEmitter::Stop() { started_ = false; }

void ParticleEmitter::Spawn(float spawn_time) {
  auto particle = new Particle();
  particle->position_ = position_;
  particle->velocity_ = initial_velocity_;

  particle_system_.AddParticle(particle);
}