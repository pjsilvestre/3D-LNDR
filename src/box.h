/*
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
  Box(const glm::vec3 &min, const glm::vec3 &max);

  glm::vec3 min() const { return corners_[0]; }
  glm::vec3 max() const { return corners_[1]; }

  glm::vec3 Center() const;
  void Draw() const;
  bool Intersect(const Ray &ray, float z_buffer_min, float z_buffer_max) const;
  bool Inside(const glm::vec3 &point) const;
  bool Inside(const std::vector<glm::vec3> &points) const;
  bool Overlap(const Box &box);

  static Box CreateMeshBoundingBox(const ofMesh &mesh);

 private:
  glm::vec3 corners_[2];
};
