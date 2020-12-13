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
  auto* particle = new Particle();

  particle->lifespan_ += ofRandom(-0.5f, 0.5f);

  particle->radius_ += ofRandom(-0.05f, 0.05f);

  // TODO encapsulate following into dedicated "CircularParticleEmitter"?
  // randomize position within a circle
  auto initial_position = position_;
  const auto circle_radius = 0.5f;

  // generate random angle between 0 and 2pi
  const auto random_angle = ofRandom(1.0f) * 2 * 3.14;

  // generate random radius between the center of the circle and its edge
  const auto random_radius = circle_radius * sqrt(ofRandom(1.0f));

  // get cartesian coordinates based on randomized radius and angle
  initial_position.x += random_radius * cos(random_angle);
  initial_position.z += random_radius * sin(random_angle);

  particle->position_ = initial_position;

  particle->velocity_ = initial_velocity_;

  particle_system_.AddParticle(particle);
}