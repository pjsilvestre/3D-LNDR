/**
 * @class Lander
 * @brief TODO
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

  bool loaded_ = false;
  bool selected_ = false;

  ofxAssimpModelLoader model_;
  Box bounds_;
  vector<Box> collision_boxes_;
};
