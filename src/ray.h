/**
 * @class Ray
 * @brief Used for optimized ray-box intersection testing
 * @details Originally described in Amy Williams, Steve Barrus, R. Keith Morley,
 * and Peter Shirley "An Efficient and Robust Ray-Box Intersection Algorithm"
 * Journal of graphics tools, 10(1):49-54, 2005.
 * @author Kevin M. Smith (CS 134 SJSU)
 * @author Patrick Silvestre
 */

#pragma once

#include "glm/vec3.hpp"

class Ray {
 public:
  Ray() = delete;
  Ray(const glm::vec3& origin, const glm::vec3& direction);

  glm::vec3 origin_;
  glm::vec3 direction_;
  glm::vec3 inverse_direction_;
  int sign_[3];
};
