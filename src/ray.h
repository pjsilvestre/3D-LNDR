#pragma once

#include "glm/vec3.hpp"

/*
 * Ray class, for use with the optimized ray-box intersection test
 * described in:
 *  Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *  "An Efficient and Robust Ray-Box Intersection Algorithm"
 *  Journal of graphics tools, 10(1):49-54, 2005
 */

class Ray {
 public:
  Ray(glm::vec3 origin, glm::vec3 direction);

  glm::vec3 origin_;
  glm::vec3 direction_;
  glm::vec3 inverse_direction_;
  int sign_[3];
};
