#include "particle-emitter.h"

void ParticleEmitter::Draw() const { particle_system_.Draw(); }

void ParticleEmitter::Update() {
  const float current_time = ofGetElapsedTimeMillis();

  if (one_shot_ && started_) {
    if (!fired_) {
      for (auto i = 0; i < group_size_; i++) {
        Spawn(current_time);
      }

      last_spawn_time_ = current_time;
    }

    fired_ = true;
    started_ = false;
  }

  if (current_time - last_spawn_time_ > 1000.0f / rate_ && started_) {
    for (auto i = 0; i < group_size_; i++) {
      Spawn(current_time);
    }

    last_spawn_time_ = current_time;
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

  particle->position_ = position_;

  // randomize velocity based on a sphere's coordinates
  const auto sphere_radius = 25.0f;
  const auto random_radius = sphere_radius * sqrt(ofRandom(1.0f));

  auto x = ofRandom(-1.0f, 1.0f);
  auto y = ofRandom(-1.0f, 1.0f);
  auto z = ofRandom(-1.0f, 1.0f);

  // normalize
  x *= 1 / sqrt(x * x + y * y + z * z);
  y *= 1 / sqrt(x * x + y * y + z * z);
  z *= 1 / sqrt(x * x + y * y + z * z);

  // grow points based on sphere's radius
  x *= random_radius;
  y *= random_radius;
  z *= random_radius;

  initial_velocity_.x = x;
  initial_velocity_.y = y;
  initial_velocity_.z = z;

  particle->velocity_ = initial_velocity_;

  particle_system_.AddParticle(particle);
}

ThrustParticleEmitter::ThrustParticleEmitter() {
  group_size_ = 100;
  rate_ = 100;
  initial_velocity_ = glm::vec3(0.0f, -10.0f, 0.0f);
}

void ThrustParticleEmitter::Spawn(float spawn_time) {
  auto* particle = new Particle();

  particle->lifespan_ += ofRandom(-0.5f, 0.5f);

  particle->radius_ = 0.1f;
  particle->radius_ += ofRandom(-0.05f, 0.05f);

  // randomize position based on a circle's coordinates
  auto initial_position = position_;
  const auto circle_radius = 0.5f;
  const auto random_radius = circle_radius * sqrt(ofRandom(1.0f));

  auto x = ofRandom(-1.0f, 1.0f);
  auto z = ofRandom(-1.0f, 1.0f);

  // normalize
  x *= 1 / sqrt(x * x + z * z);
  z *= 1 / sqrt(x * x + z * z);

  // grow points based on circle's radius
  x *= random_radius;
  z *= random_radius;

  initial_position.x += x;
  initial_position.z += z;

  particle->position_ = initial_position;

  particle->velocity_ = initial_velocity_;

  particle_system_.AddParticle(particle);
}
