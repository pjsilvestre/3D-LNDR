#include "particle-force.h"

GravityForce::GravityForce(const glm::vec3& gravity) : gravity_{gravity} {}

void GravityForce::Update(Particle& particle) { particle.forces_ += gravity_; }
