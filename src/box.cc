#include "box.h"

Box::Box(const glm::vec3 &min, const glm::vec3 &max) : corners_{min, max} {}

glm::vec3 Box::center() {
  auto center = max() - min();
  center.x /= 2;
  center.y /= 2;
  center.z /= 2;
  center += min();

  return center;
}

/**
 * @brief Determines if a point is within this Box
 * @param point The point to test
 * @return True if the point is within this Box, false otherwise
 */
bool Box::inside(const glm::vec3 &point) const {
  return (point.x >= corners_[0].x && point.x <= corners_[1].x) &&
         (point.y >= corners_[0].y && point.y <= corners_[1].y) &&
         (point.z >= corners_[0].z && point.z <= corners_[1].z);
}

/**
 * @brief Determines if multiple points are within this Box
 * @param points The points to test
 * @return True if the points are within this Box, false otherwise
 */
bool Box::inside(const std::vector<glm::vec3> &points) const {
  for (const auto &point : points) {
    if (!inside(point)) {
      return false;
    }
  }

  return true;
}

/*
 * Ray-box intersection using IEEE numerical properties to ensure that the
 * test is both robust and efficient, as described in:
 *  Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *  "An Efficient and Robust Ray-Box Intersection Algorithm"
 *  Journal of graphics tools, 10(1):49-54, 2005
 */

bool Box::intersect(const Ray &ray, const float z_buffer_min,
                    const float z_buffer_max) const {
  auto x_min =
      (corners_[ray.sign_[0]].x - ray.origin_.x) * ray.inverse_direction_.x;
  auto x_max =
      (corners_[1 - ray.sign_[0]].x - ray.origin_.x) * ray.inverse_direction_.x;
  const auto y_min =
      (corners_[ray.sign_[1]].y - ray.origin_.y) * ray.inverse_direction_.y;
  const auto y_max =
      (corners_[1 - ray.sign_[1]].y - ray.origin_.y) * ray.inverse_direction_.y;

  if (x_min > y_max || y_min > x_max) return false;

  if (y_min > x_min) x_min = y_min;
  if (y_max < x_max) x_max = y_max;

  const auto z_min =
      (corners_[ray.sign_[2]].z - ray.origin_.z) * ray.inverse_direction_.z;
  const auto z_max =
      (corners_[1 - ray.sign_[2]].z - ray.origin_.z) * ray.inverse_direction_.z;

  if (x_min > z_max || z_min > x_max) return false;

  if (z_min > x_min) x_min = z_min;
  if (z_max < x_max) x_max = z_max;

  return x_min < z_buffer_max && x_max > z_buffer_min;
}

bool Box::overlap(const Box &box) {
  // TODO
  return true;
}
