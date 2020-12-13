#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofEnableBlendMode(OF_BLENDMODE_ADD);
  ofEnableDepthTest();
  ofEnableSmoothing();
  ofSetVerticalSync(true);
  ofSetFrameRate(60);

  LoadAssets();
  SetUpCameras();
  SetUpLighting();
}

//--------------------------------------------------------------
void ofApp::LoadAssets() {
  if (background_.load("images/space.jpg")) {
    background_.resize(ofGetWidth(), ofGetHeight());
    background_.setImageType(OF_IMAGE_GRAYSCALE);
  } else {
    ofSystemAlertDialog("Background image missing. Exiting...");
    ofExit();
  }

  if (thrust_sound_player_.load("sounds/thrust.wav")) {
    thrust_sound_player_.setLoop(true);
  } else {
    ofSystemAlertDialog("Thrust sound effect missing. Exiting...");
    ofExit();
  }

  if (!gauge_font_.load("fonts/Source_Code_Pro/SourceCodePro-Black.ttf", 20)) {
    ofSystemAlertDialog("Font missing. Exiting...");
    ofExit();
  }

  if (!control_hint_font_.load("fonts/Source_Code_Pro/SourceCodePro-Black.ttf",
                               16)) {
    ofSystemAlertDialog("Font missing. Exiting...");
    ofExit();
  }

  if (mars_.loadModel("geo/mars.obj")) {
    mars_.setScaleNormalization(false);
    octree_ = Octree(mars_.getMesh(0), 10);
  } else {
    ofSystemAlertDialog("Mars model missing. Exiting...");
    ofExit();
  }

  // FIXME
  //  if (ofLoadImage(particle_texture_, "images/particle.png")) {
  //    ofDisableArbTex();
  //  } else {
  //    ofSystemAlertDialog("Particle texture missing. Exiting...");
  //    ofExit();
  //  }
  //
  //#ifdef TARGET_OPENGLES
  //  if (shader_.load("shaders-gles/shader")) shaders_loaded = true;
  //#else
  //  if (shader_.load("shaders/shader")) shaders_loaded_ = true;
  //#endif
  //
  //  if (!shaders_loaded_) {
  //    ofSystemAlertDialog("Shaders missing. Exiting...");
  //    ofExit();
  //  }
}

//--------------------------------------------------------------
void ofApp::SetUpCameras() {
  follow_cam_.setFov(67.5f);
  follow_cam_.setNearClip(0.1f);

  onboard_cam_.setFov(90.0f);
  onboard_cam_.setNearClip(0.1f);

  tracking_cam_.setFov(22.5f);
  tracking_cam_.setNearClip(0.1f);
  auto above_landing_area = landing_area_;
  above_landing_area.y += 100.0f;
  tracking_cam_.setPosition(above_landing_area);

  free_cam_.setFov(90.0f);
  free_cam_.setDistance(50.0f);
  free_cam_.setNearClip(0.1f);
}

//--------------------------------------------------------------
void ofApp::SetUpLighting() {
  ofSetSmoothLighting(true);

  landing_area_light_.setup();
  landing_area_light_.setPointLight();
  landing_area_light_.rotateDeg(270, glm::vec3(1.0f, 0.0f, 0.0f));
  auto above_landing_area = landing_area_;
  above_landing_area.y += 5.0f;
  landing_area_light_.setPosition(above_landing_area);
  landing_area_light_.enable();

  terrain_light_.setup();
  terrain_light_.setDirectional();
  terrain_light_.setPosition(0.0f, 100.0f, 0.0f);
  terrain_light_.enable();

  thruster_light_.setup();
  thruster_light_.setPointLight();
  thruster_light_.setAttenuation(1.0f, 0.5f, 0.1f);
}

//--------------------------------------------------------------
void ofApp::update() {
  UpdateCameras();
  UpdateLighting();

  current_cam_ == &free_cam_ ? ofShowCursor() : ofHideCursor();

  background_.resize(ofGetWidth(), ofGetHeight());

  lander_system_.Update(octree_);

  thruster_.position_ = lander_system_.get_position();
  thruster_.Update();
}

//--------------------------------------------------------------
void ofApp::UpdateCameras() {
  const auto lander_position = lander_system_.get_position();

  follow_cam_.orbitDeg(lander_system_.get_orientation() + 270.0f, -45.0f, 25.0f,
                       lander_position);
  onboard_cam_.orbitDeg(lander_system_.get_orientation() + 270.0f, 270.0f, 0.7f,
                        lander_position);
  tracking_cam_.lookAt(lander_position);
}

//--------------------------------------------------------------
void ofApp::UpdateLighting() {
  auto lander_position = lander_system_.get_position();
  lander_position.y -= 5.0f;

  thruster_light_.setPosition(lander_position);
  thruster_light_.orbitDeg(0.0f, 270.0f, 0.0f, lander_position);
}

//--------------------------------------------------------------
void ofApp::draw() {
  // SetUpVertexBuffer();

  ofDisableLighting();
  ofDisableDepthTest();
  ofSetColor(64, 64, 64, 256);
  background_.draw(0.0f, 0.0f);
  ofEnableDepthTest();
  ofEnableLighting();

  current_cam_->begin();

  mars_.drawFaces();

  lander_system_.Draw();

  ofSetColor(128, 128, 128, 64);
  ofDrawSphere(landing_area_, 7.0f);

  thruster_.Draw();

  // FIXME
  // ofSetColor(ofColor::white);
  // shader_.begin();
  // particle_texture_.bind();
  // vertex_buffer_.draw(GL_POINTS, 0,
  //                    thruster_.particle_system_.particles_.size());
  // particle_texture_.unbind();
  // shader_.end();

  current_cam_->end();

  ofDisableLighting();
  ofDisableDepthTest();
  if (lander_system_.altimeter_enabled()) DrawAltimeterGauge();
  DrawControlHints();
  DrawFuelGauge();
  DrawVelocityGauge();
  ofEnableDepthTest();
  ofEnableLighting();
}

////--------------------------------------------------------------
// void ofApp::SetUpVertexBuffer() {
//  // FIXME
//  if (thruster_.particle_system_.particles_.empty()) return;
//
//  vector<glm::vec3> points;
//  vector<glm::vec3> sizes;
//
//  for (auto& particle : thruster_.particle_system_.particles_) {
//    points.push_back(particle->position_);
//    sizes.push_back(glm::vec3(1.0f));
//  }
//
//  const int total = points.size();
//
//  vertex_buffer_.clear();
//  vertex_buffer_.setVertexData(&points[0], total, GL_STATIC_DRAW);
//  vertex_buffer_.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
//}

//--------------------------------------------------------------
void ofApp::DrawAltimeterGauge() const {
  const auto altimeter_message =
      "altitude: " + to_string(lander_system_.get_altitude());
  const auto bounding_box =
      gauge_font_.getStringBoundingBox(altimeter_message, 0, 0);
  ofSetColor(255, 255, 255, 180);
  gauge_font_.drawString(altimeter_message,
                         ofGetWidth() - (bounding_box.width + 50.0f),
                         bounding_box.height + 50.0f);
}

//--------------------------------------------------------------
void ofApp::DrawAxis(const glm::vec3& location) const {
  // Draw an XYZ axis in RGB at world (0,0,0) for reference.
  ofSetLineWidth(1.0);

  ofPushMatrix();
  ofTranslate(location);

  // x-axis
  ofSetColor(ofColor::red);
  ofDrawLine(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  // y-axis
  ofSetColor(ofColor::green);
  ofDrawLine(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  // z-axis
  ofSetColor(ofColor::blue);
  ofDrawLine(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::DrawControlHints() const {
  const auto control_hint =
      "| movement: wasd | thrust: space | rotation: qe | altimeter: x | follow "
      "camera: 1 | onboard camera: 2 | tracking camera: 3 | free camera: 4 | "
      "enable/disable free cam mouse: c |";
  const auto bounding_box =
      control_hint_font_.getStringBoundingBox(control_hint, 0, 0);
  ofSetColor(255, 255, 255, 180);
  control_hint_font_.drawString(
      control_hint, ofGetWidth() / 2.0f - bounding_box.width / 2.0f,
      ofGetHeight() - (bounding_box.height / 2.0f + 25.0f));
}

//--------------------------------------------------------------
void ofApp::DrawFuelGauge() const {
  string fuel_message;
  if (fuel_ < 0.0f) {
    fuel_message = "fuel: 0 seconds";
  } else {
    fuel_message = "fuel: " + to_string(static_cast<int>(fuel_)) + " seconds";
  }

  const auto bounding_box =
      gauge_font_.getStringBoundingBox(fuel_message, 0, 0);
  ofSetColor(255, 255, 255, 180);
  gauge_font_.drawString(fuel_message, 50.0f, bounding_box.height + 50.0f);
}

//--------------------------------------------------------------
void ofApp::DrawVelocityGauge() const {
  const auto velocity = lander_system_.get_velocity();
  const auto velocity_magnitude = glm::length(velocity);
  const auto velocity_message =
      "velocity: " + to_string(static_cast<int>(velocity_magnitude)) +
      " units per second";

  const auto bounding_box =
      gauge_font_.getStringBoundingBox(velocity_message, 0, 0);

  if (velocity_threshold_ < velocity_magnitude) {
    ofSetColor(255, 0, 0, 180);
  } else {
    ofSetColor(0, 255, 0, 180);
  }

  gauge_font_.drawString(velocity_message, 50.0f, bounding_box.height + 100.0f);
}

//--------------------------------------------------------------
void ofApp::keyPressed(const int key) {
  switch (key) {
    case 'W':
    case 'w':
      if (fuel_ < 0.0f) return;
      lander_system_.ForwardThrust();
      StartThrusterEffects();
      break;
    case 'A':
    case 'a':
      if (fuel_ < 0.0f) return;
      lander_system_.LeftwardThrust();
      StartThrusterEffects();
      break;
    case 'S':
    case 's':
      if (fuel_ < 0.0f) return;
      lander_system_.BackwardThrust();
      StartThrusterEffects();
      break;
    case 'D':
    case 'd':
      if (fuel_ < 0.0f) return;
      lander_system_.RightwardThrust();
      StartThrusterEffects();
      break;
    case ' ':
      if (fuel_ < 0.0f) return;
      lander_system_.UpwardThrust();
      StartThrusterEffects();
      break;
    case 'Q':
    case 'q':
      if (fuel_ < 0.0f) return;
      lander_system_.YawLeft();
      StartThrusterEffects();
      break;
    case 'E':
    case 'e':
      if (fuel_ < 0.0f) return;
      lander_system_.YawRight();
      StartThrusterEffects();
      break;
    case 'X':
    case 'x':
      if (lander_system_.altimeter_enabled()) {
        lander_system_.disable_altimeter();
      } else {
        lander_system_.enable_altimeter();
      }
    case 'C':
    case 'c':
      if (current_cam_ == &free_cam_) {
        if (free_cam_.getMouseInputEnabled()) {
          free_cam_.disableMouseInput();
        } else {
          free_cam_.enableMouseInput();
        }
      }
      break;
    case '1':
      current_cam_ = &follow_cam_;
      break;
    case '2':
      current_cam_ = &onboard_cam_;
      break;
    case '3':
      current_cam_ = &tracking_cam_;
      break;
    case '4':
      current_cam_ = &free_cam_;
      break;
    default:
      break;
  }
}
//--------------------------------------------------------------
void ofApp::StartThrusterEffects() {
  if (!thruster_light_.getIsEnabled()) thruster_light_.enable();
  if (!thrust_sound_player_.isPlaying()) thrust_sound_player_.play();
  thruster_.Start();
  fuel_ -= 1.0f / 30.0f;
}

//--------------------------------------------------------------
void ofApp::keyReleased(const int key) {
  switch (key) {
    case 'W':
    case 'w':
    case 'A':
    case 'a':
    case 'S':
    case 's':
    case 'D':
    case 'd':
    case ' ':
    case 'Q':
    case 'q':
    case 'E':
    case 'e':
      thruster_light_.disable();
      thrust_sound_player_.stop();
      thruster_.Stop();
      break;
    default:
      break;
  }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
  if (free_cam_.getMouseInputEnabled()) return;

  if (dragging_) {
    auto lander_position = lander_system_.get_position();
    const auto mouse_position =
        GetMousePointOnPlane(lander_position, free_cam_.getZAxis());
    const auto delta = mouse_position - mouse_last_pos_;

    lander_position += delta;

    lander_system_.set_position(lander_position);
    mouse_last_pos_ = mouse_position;
  }
}

//--------------------------------------------------------------
glm::vec3 ofApp::GetMousePointOnPlane(const glm::vec3& plane_origin,
                                      const glm::vec3& plane_normal) const {
  // intersect the mouse ray with the plane normal to the camera
  // return intersection point.

  // ray setup
  const auto origin = free_cam_.getPosition();
  const auto mouse_world_space =
      free_cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0));
  const auto mouse_direction = glm::normalize(mouse_world_space - origin);
  float distance;

  const auto hit = glm::intersectRayPlane(origin, mouse_direction, plane_origin,
                                          plane_normal, distance);

  if (hit) {
    // find the point of intersection on the plane. compute using parametric
    // representation of a line: p' = p + s * dir;
    const auto intersection_point = origin + distance * mouse_direction;

    return intersection_point;
  }

  return glm::vec3(0.0f);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  if (free_cam_.getMouseInputEnabled()) return;

  const auto origin = free_cam_.getPosition();
  const auto mouse_world_space =
      free_cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0));
  const auto mouse_direction = glm::normalize(mouse_world_space - origin);
  const auto hit = lander_system_.get_bounds().Intersect(
      Ray(origin, mouse_direction), 0, 10000);

  if (hit) {
    lander_system_.select();
    terrain_selected_ = false;
    dragging_ = true;
    mouse_last_pos_ = GetMousePointOnPlane(lander_system_.get_position(),
                                           free_cam_.getZAxis());
  } else {
    lander_system_.unselect();
    terrain_selected_ = true;
    dragging_ = false;
  }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) { dragging_ = false; }

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo drag_info) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}
