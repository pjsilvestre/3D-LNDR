#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofEnableDepthTest();
  ofEnableSmoothing();
  ofSetVerticalSync(true);
  ofSetFrameRate(60);

  cam_.setDistance(50.0f);
  cam_.setFov(75.0f);
  cam_.setNearClip(0.1f);

  InitializeLighting();

  mars_.loadModel("geo/mars-low-5x-v2.obj");
  mars_.setScaleNormalization(false);

  gui_.setup();
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
void ofApp::update() { lander_system_.Update(octree_); }

//--------------------------------------------------------------
void ofApp::draw() {
  ofBackground(ofColor::black);

  glDepthMask(false);
  if (gui_displayed_) gui_.draw();
  if (lander_system_.altimeter_enabled()) DrawAltimeterGauge();
  glDepthMask(true);

  cam_.begin();

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

  cam_.end();
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
    case 'C':
    case 'c':
      if (cam_.getMouseInputEnabled()) {
        cam_.disableMouseInput();
      } else {
        cam_.enableMouseInput();
      }
      break;
    case 'F':
    case 'f':
      ofToggleFullscreen();
      break;
    case 'H':
    case 'h':
      gui_displayed_ = !gui_displayed_;
      break;
    case 'R':
    case 'r':
      cam_.reset();
      break;
    case 'X':
    case 'x':
      if (lander_system_.altimeter_enabled()) {
        lander_system_.disable_altimeter();
      } else {
        lander_system_.enable_altimeter();
      }
      break;
    case OF_KEY_ALT:
      cam_.enableMouseInput();
      break;
    default:
      break;
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(const int key) {
  switch (key) {
    case OF_KEY_ALT:
      cam_.disableMouseInput();
      break;
    default:
      break;
  }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
  if (cam_.getMouseInputEnabled()) return;

  if (dragging_) {
    auto lander_position = lander_system_.get_position();
    const auto mouse_position =
        GetMousePointOnPlane(lander_position, cam_.getZAxis());
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
  const auto origin = cam_.getPosition();
  const auto mouse_world_space =
      cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0));
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
  if (cam_.getMouseInputEnabled()) return;

  if (lander_system_.is_loaded()) {
    const auto origin = cam_.getPosition();
    const auto mouse_world_space =
        cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0));
    const auto mouse_direction = glm::normalize(mouse_world_space - origin);
    const auto hit = lander_system_.get_bounds().Intersect(
        Ray(origin, mouse_direction), 0, 10000);

    if (hit) {
      lander_system_.select();
      terrain_selected_ = false;
      dragging_ = true;
      mouse_last_pos_ =
          GetMousePointOnPlane(lander_system_.get_position(), cam_.getZAxis());
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
