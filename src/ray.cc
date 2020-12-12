#include "ray.h"

/**
 * @brief Creates a ray
 * @param origin The origin of the ray
 * @param direction The direction of the ray
 */
Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
    : direction_{direction},
      inverse_direction_{1 / direction.x, 1 / direction.y, 1 / direction.z},
      origin_{origin} {
  sign_[0] = inverse_direction_.x < 0;
  sign_[1] = inverse_direction_.y < 0;
  sign_[2] = inverse_direction_.z < 0;
}
