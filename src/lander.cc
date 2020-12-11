#include "lander.h"

Lander::Lander() {
  if (model_.loadModel("geo/lander.obj")) {
    loaded_ = true;
    model_.setScaleNormalization(false);
    model_.setPosition(position_.x, position_.y, position_.z);
    position_ = glm::vec3(0.0f, 10.0f, 0.0f);

    bounds_ =
        Box(model_.getSceneMin() + position_, model_.getSceneMax() + position_);
  } else {
    cerr << "could not find geo/lander.obj" << endl;
  }
}

void Lander::Update(const Octree& octree) {
  if (loaded_) {
    bounds_ =
        Box(model_.getSceneMin() + position_, model_.getSceneMax() + position_);
    transformation_matrix_ = glm::translate(glm::mat4(1.0f), position_);
    collision_boxes_.clear();
    octree.Intersect(bounds_, octree.root_, collision_boxes_);

    if (altimeter_enabled_) {
      const auto ray_origin = position_;
      const auto ray_direction = glm::vec3(0.0f, -1.0f, 0.0f);
      const auto lander_ray = Ray(ray_origin, ray_direction);
      const auto point_selected =
          octree.Intersect(lander_ray, octree.root_, selected_node_);

      if (point_selected) {
        altitude_ = glm::length(position_ - terrain_point_);
        terrain_point_selected_ = true;
        terrain_point_ = octree.mesh_.getVertex(selected_node_.points_[0]);
      } else {
        terrain_point_selected_ = false;
        terrain_point_ = glm::vec3(-10000.0f);
      }
    }
  }
}

void Lander::Draw() {
  ofPushMatrix();
  ofMultMatrix(transformation_matrix_);

  model_.drawFaces();

  ofPopMatrix();

  if (selected_) {
    DrawBounds();
    DrawCollisionBoxes();
  }

  if (altimeter_enabled_ && terrain_point_selected_) {
    ofSetColor(ofColor::green);
    ofDrawSphere(terrain_point_, 0.5f);
  }
}

void Lander::DrawBounds() const {
  ofSetColor(ofColor::white);
  bounds_.Draw();
}

void Lander::DrawCollisionBoxes() const {
  ofNoFill();
  ofSetColor(ofColor::hotPink);

  for (const auto& box : collision_boxes_) {
    box.Draw();
  }
}
