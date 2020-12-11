#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofEnableDepthTest();
  ofEnableSmoothing();
  ofSetVerticalSync(true);
  ofSetFrameRate(60);

  cam_.disableMouseInput();
  cam_.setDistance(10);
  cam_.setFov(65.5f);  // approx equivalent to 28mm in 35mm format
  cam_.setNearClip(0.1f);

  InitializeLighting();

  mars_.loadModel("geo/mars-low-5x-v2.obj");
  mars_.setScaleNormalization(false);

  const auto max_num_octree_levels = 10;
  gui_.setup();
  gui_.add(num_octree_levels_.setup("Number of Octree Levels", 1, 1,
                                    max_num_octree_levels));

  cout << "creating octree..." << endl;
  const auto octree_creation_start = ofGetElapsedTimeMillis();
  octree_ = Octree(mars_.getMesh(0), max_num_octree_levels);
  const auto octree_creation_finish = ofGetElapsedTimeMillis();
  const auto octree_creation_time =
      octree_creation_finish - octree_creation_start;
  cout << "octree created in " << octree_creation_time << "ms ("
       << octree_creation_time / 1000.0f << "s)" << endl;
  cout << "number of terrain vertices: " << mars_.getMesh(0).getNumVertices()
       << endl;
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
  glDepthMask(true);

  cam_.begin();

  if (terrain_selected_) {
    DrawAxis(glm::vec3(0.0f));
  } else {
    DrawAxis(lander_system_.get_position());
  }

  ofEnableLighting();
  mars_.drawFaces();

  if (lander_system_.is_loaded()) {
    lander_system_.Draw();
  }

  if (terrain_points_displayed_) DrawTerrainPoints();

  if (point_selected_) DrawPointSelected();

  if (octree_displayed_) DrawOctree();

  cam_.end();
}

//--------------------------------------------------------------
void ofApp::DrawAxis(const glm::vec3& location) {
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
void ofApp::DrawTerrainPoints() {
  glPointSize(3.0f);
  ofSetColor(ofColor::green);
  mars_.drawVertices();
}

//--------------------------------------------------------------
void ofApp::DrawPointSelected() {
  const auto point = octree_.mesh_.getVertex(selected_node_.points_[0]);
  const auto direction = point - cam_.getPosition();
  ofSetColor(ofColor::green);
  ofDrawSphere(point, .02f * glm::length(direction));
}

//--------------------------------------------------------------
void ofApp::DrawOctree() {
  ofDisableLighting();

  if (leaf_nodes_displayed_) {
    // octree_.drawLeafNodes(octree_.root_);
    // cout << "number of leaf nodes: " << octree_.number_of_leaves_ << endl;
    cerr << "leaf_nodes_displayed_ not implemented" << endl;
  } else {
    ofNoFill();
    ofSetColor(ofColor::white);
    octree_.Draw(num_octree_levels_, 0);
  }

  ofEnableLighting();
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
    case 'L':
    case 'l':
      leaf_nodes_displayed_ = !leaf_nodes_displayed_;
      break;
    case 'O':
    case 'o':
      octree_displayed_ = !octree_displayed_;
      break;
    case 'R':
    case 'r':
      cam_.reset();
      break;
    case 'T':
    case 't':
      SetCameraTarget();
      break;
    case 'U':
    case 'u':
      break;
    case 'V':
    case 'v':
      TogglePointsDisplay();
      break;
    case OF_KEY_ALT:
      cam_.enableMouseInput();
      alt_key_down_ = true;
      break;
    case OF_KEY_CONTROL:
      ctrl_key_down_ = true;
      break;
    case OF_KEY_SHIFT:
      break;
    case OF_KEY_DEL:
      break;
    default:
      break;
  }
}

//--------------------------------------------------------------
void ofApp::TogglePointsDisplay() {
  terrain_points_displayed_ = !terrain_points_displayed_;
}

//--------------------------------------------------------------
void ofApp::ToggleSelectTerrain() { terrain_selected_ = !terrain_selected_; }

//--------------------------------------------------------------
void ofApp::keyReleased(const int key) {
  switch (key) {
    case OF_KEY_ALT:
      cam_.disableMouseInput();
      alt_key_down_ = false;
      break;
    case OF_KEY_CONTROL:
      ctrl_key_down_ = false;
      break;
    case OF_KEY_SHIFT:
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
  } else {
    glm::vec3 unused_point;
    SelectOctreeNode(unused_point);
  }
}

//--------------------------------------------------------------
glm::vec3 ofApp::GetMousePointOnPlane(const glm::vec3& plane_origin,
                                      const glm::vec3& plane_normal) {
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
bool ofApp::SelectOctreeNode(glm::vec3& return_point) {
  const auto mouse = glm::vec3(mouseX, mouseY, 0.0f);
  const auto ray_origin = cam_.screenToWorld(mouse);
  const auto ray_direction = glm::normalize(ray_origin - cam_.getPosition());
  const auto mouse_ray = Ray(ray_origin, ray_direction);

  const auto before_point_selected = ofGetElapsedTimeMillis();
  point_selected_ = octree_.Intersect(mouse_ray, octree_.root_, selected_node_);
  const auto after_point_selected = ofGetElapsedTimeMillis();
  const auto selection_time = after_point_selected - before_point_selected;

  if (point_selected_) {
    return_point = octree_.mesh_.getVertex(selected_node_.points_[0]);
    cout << "point selected in " << selection_time << "ms ("
         << selection_time / 1000.0f << "s)" << endl;
  }

  return point_selected_;
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
  } else {
    glm::vec3 unused_point;
    SelectOctreeNode(unused_point);
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
void ofApp::dragEvent(ofDragInfo dragInfo) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
bool ofApp::MouseIntersectPlane(const glm::vec3& plane_point,
                                const glm::vec3& plane_normal,
                                glm::vec3& intersection_point) {
  const auto ray_origin = cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0.0f));
  const auto ray_direction = glm::normalize(ray_origin - cam_.getPosition());

  return Utility::RayIntersectPlane(ray_origin, ray_direction, plane_point,
                                    plane_normal, intersection_point);
}

//--------------------------------------------------------------
void ofApp::SetCameraTarget() {
  // Set the camera to use the selected point as it's new target
}
