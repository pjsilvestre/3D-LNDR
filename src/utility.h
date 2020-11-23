/**
 * @class Utility
 * @brief Utility class with miscellaneous functions
 * @author Kevin M. Smith (CS 134 SJSU)
 * @author Patrick Silvestre
 */

#pragma once

#include "glm/geometric.hpp"
#include "glm/vec3.hpp"

class Utility {
 public:
  static bool RayIntersectPlane(const glm::vec3& ray_origin,
                                const glm::vec3& ray_direction,
                                const glm::vec3& plane_point,
                                const glm::vec3& plane_normal,
                                glm::vec3& intersection_point);

  static glm::vec3 ReflectVector(const glm::vec3& vector,
                                 const glm::vec3& normal);
};
