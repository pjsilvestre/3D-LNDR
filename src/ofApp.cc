#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofEnableDepthTest();
  ofEnableSmoothing();
  ofSetVerticalSync(true);
  ofSetFrameRate(60);

  SetUpLighting();
  SetUpCameras();

  if (background_.load("space.jpg")) {
    background_loaded_ = true;
    background_.resize(ofGetWidth(), ofGetHeight());
    background_.setImageType(OF_IMAGE_GRAYSCALE);
  }

  mars_.loadModel("geo/mars-low-5x-v2.obj");
  mars_.setScaleNormalization(false);

  octree_ = Octree(mars_.getMesh(0), 10);
}

//--------------------------------------------------------------
void ofApp::SetUpLighting() {
  ofSetSmoothLighting(true);

  lander_point_light_.setup();
  lander_point_light_.setPointLight();
  lander_point_light_.setAttenuation(1.0f, 0.01f, 0.01f);
  lander_point_light_.enable();

  landing_area_point_light_.setup();
  landing_area_point_light_.setPointLight();
  landing_area_point_light_.rotateDeg(270, glm::vec3(1.0f, 0.0f, 0.0f));
  auto above_landing_area = landing_area_;
  above_landing_area.y += 5.0f;
  landing_area_point_light_.setPosition(above_landing_area);
  landing_area_point_light_.enable();

  terrain_overhead_directional_light_.setup();
  terrain_overhead_directional_light_.setDirectional();
  terrain_overhead_directional_light_.setPosition(0.0f, 100.0f, 0.0f);
  terrain_overhead_directional_light_.enable();
}

//--------------------------------------------------------------
void ofApp::SetUpCameras() {
  follow_cam_.setFov(67.5f);
  follow_cam_.setNearClip(0.1f);

  onboard_cam_.setFov(90.0f);
  onboard_cam_.setNearClip(0.1f);

  tracking_cam_.setFov(22.5f);
  auto above_landing_area = landing_area_;
  above_landing_area.y += 100.0f;
  tracking_cam_.setPosition(above_landing_area);

  free_cam_.setFov(90.0f);
  free_cam_.setDistance(50.0f);
  free_cam_.setNearClip(0.1f);
}

//--------------------------------------------------------------
void ofApp::update() {
  if (background_loaded_) background_.resize(ofGetWidth(), ofGetHeight());

  const auto lander_position = lander_system_.get_position();

  follow_cam_.orbitDeg(lander_system_.get_orientation() + 270.0f, -45.0f, 25.0f,
                       lander_position);
  onboard_cam_.orbitDeg(lander_system_.get_orientation() + 270.0f, 270.0f, 0.7f,
                        lander_position);
  tracking_cam_.lookAt(lander_position);

  lander_point_light_.setPosition(lander_position);
  lander_point_light_.orbitDeg(0.0f, 270.0f, 0.0f, lander_position);

  lander_system_.Update(octree_);
}

//--------------------------------------------------------------
void ofApp::draw() {
  ofBackground(ofColor::black);

  if (background_loaded_) {
    glDepthMask(false);
    ofSetColor(64, 64, 64, 256);
    background_.draw(0.0f, 0.0f);
    glDepthMask(true);
  }

  current_cam_->begin();

  ofEnableLighting();
  mars_.drawFaces();

  if (lander_system_.is_loaded()) {
    lander_system_.Draw();
  }

  ofSetColor(128, 128, 128, 64);
  ofDrawSphere(landing_area_, 7.0f);

  current_cam_->end();

  ofDisableLighting();
  if (gui_displayed_) gui_.draw();
  if (lander_system_.altimeter_enabled()) DrawAltimeterGauge();
  DrawControlHints();
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
void ofApp::DrawControlHints() const {
  const auto control_hint =
      "| movement: wasd | thrust: space | rotation: qe | altimeter: x | follow "
      "camera: 1 | onboard camera: 2 | tracking camera: 3 | free camera: 4 | "
      "enable/disable free cam mouse: c";
  const ofBitmapFont font;
  const auto bounding_box = font.getBoundingBox(control_hint, 0, 0);
  ofSetColor(ofColor::white);
  ofDrawBitmapString(control_hint, 10.0f,
                     ofGetHeight() - bounding_box.height / 2.0f);
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
