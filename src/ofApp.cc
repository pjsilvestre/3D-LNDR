#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofEnableDepthTest();
  ofEnableSmoothing();
  ofSetVerticalSync(true);
  ofSetFrameRate(60);

  InitializeLighting();

  follow_cam_.setFov(67.5f);

  onboard_cam_.setNearClip(0.1f);
  onboard_cam_.setFov(90.0f);

  tracking_cam_.setFov(22.5f);
  tracking_cam_.setPosition(-75.0f, 50.0f, 0.0f);

  if (background_.load("space.jpg")) {
    background_loaded_ = true;
    background_.setImageType(OF_IMAGE_GRAYSCALE);
    background_.resize(ofGetWidth(), ofGetHeight());
  }

  mars_.loadModel("geo/mars-low-5x-v2.obj");
  mars_.setScaleNormalization(false);

  octree_ = Octree(mars_.getMesh(0), 10);
}

//--------------------------------------------------------------
void ofApp::InitializeLighting() {
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);

  float ambient[]{0.5f, 0.5f, 0.5, 1.0f};
  float diffuse[]{1.0f, 1.0f, 1.0f, 1.0f};
  float position[]{5.0f, 5.0f, 5.0f, 0.0f};

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHT0);

  // glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
  // glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  // glLightfv(GL_LIGHT1, GL_POSITION, position);
  // glEnable(GL_LIGHT1);

  float lander_model_ambient[]{1.0f, 1.0f, 1.0f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lander_model_ambient);

  float lander_model_two_side[]{GL_TRUE};
  glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lander_model_two_side);
}

//--------------------------------------------------------------
void ofApp::update() {
  if (background_loaded_) background_.resize(ofGetWidth(), ofGetHeight());

  follow_cam_.orbitDeg(lander_system_.get_orientation() + 270.0f, -45.0f, 25.0f,
                       lander_system_.get_position());

  onboard_cam_.orbitDeg(lander_system_.get_orientation() + 270.0f, 270.0f, 0.7f,
                        lander_system_.get_position());

  tracking_cam_.lookAt(lander_system_.get_position());

  lander_system_.Update(octree_);
}

//--------------------------------------------------------------
void ofApp::draw() {
  ofBackground(ofColor::black);

  if (background_loaded_) {
    glDepthMask(false);
    ofSetColor(255, 255, 255, 100);
    background_.draw(0.0f, 0.0f);
    glDepthMask(true);
  }

  current_cam_->begin();

  if (terrain_selected_) {
    DrawAxis(glm::vec3(0.0f));
  } else {
    if (lander_system_.is_loaded()) {
      DrawAxis(lander_system_.get_position());
    }
  }

  ofEnableLighting();
  mars_.drawFaces();

  if (lander_system_.is_loaded()) {
    lander_system_.Draw();
  }

  current_cam_->end();

  if (gui_displayed_) gui_.draw();
  if (lander_system_.altimeter_enabled()) DrawAltimeterGauge();
}

//--------------------------------------------------------------
void ofApp::DrawAltimeterGauge() const {
  // TODO replace bitmap string with truetype string

  const auto altimeter_message =
      "altitude: " + to_string(lander_system_.get_altitude());
  const ofBitmapFont font;
  const auto bounding_box = font.getBoundingBox(altimeter_message, 0, 0);
  ofSetColor(ofColor::white);
  ofDrawBitmapString(
      altimeter_message,
      glm::vec3(ofGetWidth() - (bounding_box.width / 2.0f + 80.0f),
                10.0f + bounding_box.height / 2.0f, 0.0f));
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
void ofApp::keyPressed(const int key) {
  switch (key) {
    case 'W':
    case 'w':
      lander_system_.ForwardThrust();
      break;
    case 'A':
    case 'a':
      lander_system_.LeftwardThrust();
      break;
    case 'S':
    case 's':
      lander_system_.BackwardThrust();
      break;
    case 'D':
    case 'd':
      lander_system_.RightwardThrust();
      break;
    case ' ':
      lander_system_.UpwardThrust();
      break;
    case 'Q':
    case 'q':
      lander_system_.YawLeft();
      break;
    case 'E':
    case 'e':
      lander_system_.YawRight();
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
      if (free_cam_.getMouseInputEnabled()) {
        free_cam_.disableMouseInput();
      } else {
        free_cam_.enableMouseInput();
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
    case 'F':
    case 'f':
      ofToggleFullscreen();
      break;
    case 'H':
    case 'h':
      gui_displayed_ = !gui_displayed_;
      break;
      break;
    default:
      break;
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(const int key) {
  switch (key) {
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

  if (lander_system_.is_loaded()) {
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
