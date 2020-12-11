#include "particle-force.h"

GravityForce::GravityForce(const glm::vec3& gravity) : gravity_{gravity} {}

void GravityForce::Update(Particle* particle) {
  if (particle == nullptr) {
    cerr << "GravityForce::Update() argument is null" << endl;
  }

  particle->forces_ += gravity_;
}
