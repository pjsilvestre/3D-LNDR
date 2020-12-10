/**
 * @class Lander
 * @brief TODO
 * @author Patrick Silvestre
 */

#pragma once

#include "box.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class Lander {
 public:
  Lander();

  void Update();
  void Draw();
  void DrawBounds();
  void DrawCollisionBoxes();

  bool is_loaded() const { return loaded_; }
  void select() { selected_ = true; }
  void unselect() { selected_ = false; }
  glm::vec3 get_position() const { return position_; }
  void set_position(const glm::vec3& position) { position_ = position; }
  Box get_bounds() const { return bounds_; }
  glm::mat4 get_transformation_matrix() const { return transformation_matrix_; }
  void clear_collision_boxes();
  void set_collision_boxes(const vector<Box>& terrain_collision_boxes) {
    collision_boxes_ = terrain_collision_boxes;
  }

 private:
  bool loaded_ = false;
  bool selected_ = false;

  ofxAssimpModelLoader model_;
  glm::vec3 position_ = glm::vec3(0.0f);
  glm::vec3 velocity_;
  glm::vec3 acceleration_;
  glm::mat4 transformation_matrix_;
  Box bounds_;
  vector<Box> collision_boxes_;
};
