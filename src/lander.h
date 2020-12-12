/**
 * @class Lander
 * @brief Encapsulates a Lander, which is a Particle with an associated Lander
 * model and associated functionality
 * @author Patrick Silvestre
 */

#pragma once

#include "box.h"
#include "octree.h"
#include "ofxAssimpModelLoader.h"
#include "particle.h"

class Octree;

class Lander : public Particle {
 public:
  Lander();

  void Update(const Octree& octree);
  void Draw();
  void DrawBounds() const;
  void DrawCollisionBoxes() const;

  bool altimeter_enabled_ = false;
  bool loaded_ = false;
  bool selected_ = false;
  bool terrain_point_selected_ = false;
  float altitude_ = 0.0f;

  ofxAssimpModelLoader model_;
  glm::vec3 terrain_point_;
  Box bounds_;
  vector<Box> collision_boxes_;
  TreeNode selected_node_;
};
