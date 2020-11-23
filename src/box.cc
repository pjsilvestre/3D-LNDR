#include "box.h"

/**
 * @brief Creates a Box
 * @param min The desired minimum corner
 * @param max The desired maximum corner
 */
Box::Box(const glm::vec3& min, const glm::vec3& max) : corners_{min, max} {}

/**
 * @brief Gets the center of this Box
 * @return A vector representing the center of this Box
 */
glm::vec3 Box::Center() const {
  auto center = max() - min();
  center.x /= 2;
  center.y /= 2;
  center.z /= 2;
  center += min();

  return center;
}
/**
 * @brief Draws this Box's wireframe
 */
void Box::Draw() const {
  const auto& min_corner = corners_[0];
  const auto& max_corner = corners_[1];
  const auto size = max_corner - min_corner;
  const auto center = size / 2 + min_corner;

  ofNoFill();
  ofDrawBox(center, size.x, size.y, size.z);
}

/**
 * @brief Determines if a point is within this Box
 * @param point The point to test
 * @return True if the point is within this Box, false otherwise
 */
bool Box::Inside(const glm::vec3& point) const {
  return (point.x >= corners_[0].x && point.x <= corners_[1].x) &&
         (point.y >= corners_[0].y && point.y <= corners_[1].y) &&
         (point.z >= corners_[0].z && point.z <= corners_[1].z);
}

/**
 * @brief Determines if multiple points are within this Box
 * @param points The points to test
 * @return True if the points are within this Box, false otherwise
 */
bool Box::Inside(const std::vector<glm::vec3>& points) const {
  for (const auto& point : points) {
    if (!Inside(point)) {
      return false;
    }
  }

  return true;
}

/**
 * @brief Determines if a ray intersects this Box
 * @details Determines if a ray this Box using IEEE numerical properties to
 * ensure that the test is both robust and efficient, as described in Amy
 * Williams, Steve Barrus, R. Keith Morley, and Peter Shirley "An Efficient and
 * Robust Ray-Box Intersection Algorithm" Journal of graphics tools,
 * 10(1):49-54, 2005
 * @param ray The ray to test
 * @param z_buffer_min The minimum z-buffer distance for culling
 * @param z_buffer_max The maximum z-buffer distance for culling
 * @return True if the ray intersects this Box, false otherwise
 */
bool Box::Intersect(const Ray& ray, const float z_buffer_min,
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

/**
 * @brief Determines if this Box overlaps with another Box
 * @param other_box The other Box to check
 * @return True if this Box overlaps the other Box, false otherwise
 */
bool Box::Overlap(const Box& other_box) const {
  if (min().x < other_box.max().x && other_box.min().x < max().x) {
    if (min().y < other_box.max().y && other_box.min().y < max().y) {
      if (min().z < other_box.max().z && other_box.min().z < max().z) {
        return true;
      }
    }
  }

  return false;
}

/**
 * @brief Creates a bounding Box for a given mesh
 * @param mesh The given mesh to bound
 * @return The mesh's bounding Box
 */
Box Box::CreateMeshBoundingBox(const ofMesh& mesh) {
  // return a Mesh Bounding Box for the entire Mesh
  const auto num_vertices = mesh.getNumVertices();
  const auto first_vertex = mesh.getVertex(0);
  auto max = first_vertex;
  auto min = first_vertex;

  for (auto i = 1; i < num_vertices; i++) {
    const auto current_vertex = mesh.getVertex(i);

    if (current_vertex.x > max.x)
      max.x = current_vertex.x;
    else if (current_vertex.x < min.x)
      min.x = current_vertex.x;

    if (current_vertex.y > max.y)
      max.y = current_vertex.y;
    else if (current_vertex.y < min.y)
      min.y = current_vertex.y;

    if (current_vertex.z > max.z)
      max.z = current_vertex.z;
    else if (current_vertex.z < min.z)
      min.z = current_vertex.z;
  }

  return Box{min, max};
}
