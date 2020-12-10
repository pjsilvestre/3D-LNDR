#include "lander.h"

Lander::Lander() {
  if (model_.loadModel("geo/lander.obj")) {
    loaded_ = true;
    model_.setScaleNormalization(false);
    model_.setPosition(position_.x, position_.y, position_.z);

    bounds_ =
        Box(model_.getSceneMin() + position_, model_.getSceneMax() + position_);
  } else {
    cerr << "could not find geo/lander.obj" << endl;
  }
}

void Lander::Update() {
  if (loaded_) {
    bounds_ =
        Box(model_.getSceneMin() + position_, model_.getSceneMax() + position_);
    transformation_matrix_ = glm::translate(glm::mat4(1.0f), position_);

    Integrate();
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
}

void Lander::UpwardThrust() { acceleration_ = glm::vec3(0.0f, 1.0f, 0.0f); }

void Lander::ForwardThrust() { acceleration_ = glm::vec3(1.0f, 0.0f, 0.0f); }

void Lander::BackwardThrust() { acceleration_ = glm::vec3(-1.0f, 0.0f, 0.0f); }

void Lander::LeftwardThrust() { acceleration_ = glm::vec3(0.0f, 0.0f, -1.0f); }

void Lander::RightwardThrust() { acceleration_ = glm::vec3(0.0f, 0.0f, 1.0f); }

//-Private Methods----------------------------------------------

void Lander::DrawBounds() {
  ofSetColor(ofColor::white);
  bounds_.Draw();
}

void Lander::DrawCollisionBoxes() {
  ofNoFill();
  ofSetColor(ofColor::hotPink);

  for (const auto& box : collision_boxes_) {
    box.Draw();
  }
}

void Lander::clear_collision_boxes() { collision_boxes_.clear(); }

void Lander::Integrate() {
  position_ += velocity_ * 1 / 60;
  velocity_ += acceleration_ * 1 / 60;
  velocity_ *= 0.99f;
  acceleration_ *= 0.99f;
}
