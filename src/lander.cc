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

void Lander::Update(const Octree& octree) {
  if (loaded_) {
    bounds_ =
        Box(model_.getSceneMin() + position_, model_.getSceneMax() + position_);
    transformation_matrix_ = glm::translate(glm::mat4(1.0f), position_);
    collision_boxes_.clear();
    octree.Intersect(bounds_, octree.root_, collision_boxes_);

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

void Lander::UpwardThrust() {
  acceleration_ = glm::vec3(0.0f, Constants::kLanderInitialAcceleration, 0.0f);
}

void Lander::ForwardThrust() {
  acceleration_ = glm::vec3(Constants::kLanderInitialAcceleration, 0.0f, 0.0f);
}

void Lander::BackwardThrust() {
  acceleration_ = glm::vec3(-Constants::kLanderInitialAcceleration, 0.0f, 0.0f);
}

void Lander::LeftwardThrust() {
  acceleration_ = glm::vec3(0.0f, 0.0f, -Constants::kLanderInitialAcceleration);
}

void Lander::RightwardThrust() {
  acceleration_ = glm::vec3(0.0f, 0.0f, Constants::kLanderInitialAcceleration);
}

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

void Lander::Integrate() {
  // FIXME replace 1 / 60 with either:
  // - 1 / ofGetFrameRate() -> Lander disappears
  // - 1 / kTargetFrameRate -> Lander does not move

  position_ += velocity_ * 1 / 60;
  velocity_ += acceleration_ * 1 / 60;
  velocity_ *= Constants::kLanderVelocityDamping;
  acceleration_ *= Constants::kLanderAccelerationDamping;
}
