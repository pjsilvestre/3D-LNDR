/**
 * @class Box
 * @brief Axis-aligned bounding box for use in optimized ray-box intersection
 * testing
 * @details Originally described in Amy Williams, Steve Barrus, R. Keith Morley,
 * and Peter Shirley "An Efficient and Robust Ray-Box Intersection Algorithm"
 * Journal of graphics tools, 10(1):49-54, 2005.
 * @author Kevin M. Smith (CS 134 SJSU)
 * @author Patrick Silvestre
 */

#pragma once

#include "ofMain.h"
#include "ray.h"

class Box {
 public:
  Box() = default;
  Box(const glm::vec3& min_corner, const glm::vec3& max_corner);

  glm::vec3 get_min_corner() const { return corners_[0]; }
  glm::vec3 get_max_corner() const { return corners_[1]; }

  glm::vec3 Center() const;
  void Draw() const;
  bool Intersect(const Ray& ray, float z_buffer_min, float z_buffer_max) const;
  bool Inside(const glm::vec3& point) const;
  bool Inside(const std::vector<glm::vec3>& points) const;
  bool Overlap(const Box& other_box) const;

  static Box CreateMeshBoundingBox(const ofMesh& mesh);

 private:
  glm::vec3 corners_[2] = {glm::vec3(0.0f), glm::vec3(0.0f)};
  // note: corners_[0] = min_corner, corners_[1] = max_corner
  // I would create min_corner and max_corner data
  // members, but this would require non-trivial
  // refactoring of Ray::Intersect()
};
