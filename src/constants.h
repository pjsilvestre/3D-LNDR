#pragma once

namespace constants {
// General constants
constexpr int kTargetFrameRate = 60;

// Particle constants
constexpr float kParticleAngularAccelerationDamping = 0.99f;
constexpr float kParticleAngularVelocityDamping = 0.99f;
constexpr float kParticleAccelerationDamping = 0.99f;
constexpr float kParticleInitialAcceleration = 1.0f;
constexpr float kParticleInitialAngularAcceleration = 10.0f;
constexpr float kParticleTerminalAngularVelocity = 15.0f;
constexpr float kParticleTerminalVelocity = 5.0f;
constexpr float kParticleVelocityDamping = 0.99f;
};  // namespace constants
