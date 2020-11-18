#include "utility.h"

/**
 * @brief Determines if a ray intersects a plane.
 * @param ray_origin The ray's origin
 * @param ray_direction The ray's direction
 * @param plane_point A point on the plane
 * @param plane_normal A normal to the plane
 * @param intersection_point The intersection point, if applicable
 * @return True if the ray intersects the plane, false otherwise
 */
bool RayIntersectPlane(const glm::vec3& ray_origin,
                       const glm::vec3& ray_direction,
                       const glm::vec3& plane_point,
                       const glm::vec3& plane_normal,
                       glm::vec3& intersection_point) {
  const auto epsilon = .000000001f;

  const auto d1 = dot(plane_point - ray_origin, plane_normal);
  // if d1 is 0, then the ray is on the plane or there is no intersection
  if (abs(d1) < epsilon) return false;

  const auto d2 = dot(ray_direction, plane_normal);
  //  if d2 is 0, then the ray is parallel to the plane
  if (abs(d2) < epsilon) return false;

  //  compute the intersection point and return it in "intersection_point"
  intersection_point = (d1 / d2) * ray_direction + ray_origin;
  return true;
}

/**
 * @brief Computes the reflection of a vector incident on a surface at the
 * normal
 * @param vector The vector to reflect
 * @param normal The surface normal
 * @return The reflected vector
 */
glm::vec3 ReflectVector(const glm::vec3& vector, const glm::vec3& normal) {
  return vector - 2 * dot(vector, normal) * normal;
}