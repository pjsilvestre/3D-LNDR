#include "particle-force.h"

//-Gravity Force Implementation---------------------------------
GravityForce::GravityForce(const glm::vec3& gravity) : gravity_{gravity} {}

void GravityForce::Update(Particle* particle) {
  if (particle == nullptr) {
    cerr << "GravityForce::Update() argument is null" << endl;
    return;
  }

  particle->forces_ += gravity_;
}

//-TurbulenceForce Implementation-------------------------------
TurbulenceForce::TurbulenceForce(const glm::vec3& min_turbulence,
                                 const glm::vec3& max_turbulence)
    : min_turbulence_{min_turbulence}, max_turbulence_{max_turbulence} {}

void TurbulenceForce::Update(Particle* particle) {
  particle->forces_.x += ofRandom(min_turbulence_.x, max_turbulence_.x);
  particle->forces_.y += ofRandom(min_turbulence_.y, max_turbulence_.y);
  particle->forces_.z += ofRandom(min_turbulence_.z, max_turbulence_.z);
}

XZTurbulenceForce::XZTurbulenceForce(const glm::vec3& min_turbulence,
                                     const glm::vec3& max_turbulence)
    : TurbulenceForce(min_turbulence, max_turbulence) {}

void XZTurbulenceForce::Update(Particle* particle) {
  particle->forces_.x += ofRandom(min_turbulence_.x, max_turbulence_.x);
  particle->forces_.z += ofRandom(min_turbulence_.z, max_turbulence_.z);
}
