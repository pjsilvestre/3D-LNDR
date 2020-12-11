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
  forces_ += glm::vec3(0.0f, constants::kParticleInitialAcceleration, 0.0f);
}

void Lander::ForwardThrust() {
  forces_  += glm::vec3(constants::kParticleInitialAcceleration, 0.0f, 0.0f);
}

void Lander::BackwardThrust() {
  forces_ += glm::vec3(-constants::kParticleInitialAcceleration, 0.0f, 0.0f);
}

void Lander::LeftwardThrust() {
  forces_ += glm::vec3(0.0f, 0.0f, -constants::kParticleInitialAcceleration);
}

void Lander::RightwardThrust() {
  forces_ += glm::vec3(0.0f, 0.0f, constants::kParticleInitialAcceleration);
}

//-Private Methods----------------------------------------------

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
