// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppMemberFunctionMayBeConst

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofEnableDepthTest();
  ofEnableSmoothing();
  ofSetVerticalSync(true);

  cam_.disableMouseInput();
  cam_.setDistance(10);
  cam_.setFov(65.5f);  // approx equivalent to 28mm in 35mm format
  cam_.setNearClip(0.1f);

  InitializeLighting();

  mars_.loadModel("geo/mars-low-5x-v2.obj");
  mars_.setScaleNormalization(false);

  const auto max_num_octree_levels{10};
  gui_.setup();
  gui_.add(num_octree_levels_.setup("Number of Octree Levels", 1, 1,
                                    max_num_octree_levels));

  octree_ = Octree(mars_.getMesh(0), max_num_octree_levels);
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
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  ofBackground(ofColor::black);

  glDepthMask(false);
  if (gui_displayed_) gui_.draw();
  glDepthMask(true);

  cam_.begin();
  ofPushMatrix();

  if (terrain_selected_) {
    DrawAxis(glm::vec3(0.0f));
  } else {
    DrawAxis(lander_.getPosition());
  }

  if (wireframe_enabled_) {
    ofDisableLighting();
    DrawWireframes();
  } else {
    ofEnableLighting();
    mars_.drawFaces();

    if (lander_loaded_) {
      lander_.drawFaces();

      if (lander_bounding_boxes_displayed_) DrawLanderBoundingBoxes();

      if (lander_selected_) {
        DrawLanderBounds();
        DrawLanderCollisionBoxes();
      }
    }
  }

  if (terrain_points_displayed_) DrawTerrainPoints();

  if (point_selected_) DrawPointSelected();

  if (octree_displayed_) DrawOctree();

  ofPopMatrix();
  cam_.end();
}

//--------------------------------------------------------------
void ofApp::DrawWireframes() {
  ofSetColor(ofColor::slateGray);

  mars_.drawWireframe();

  if (lander_loaded_) {
    lander_.drawWireframe();
  }
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
void ofApp::DrawLanderBoundingBoxes() {
  ofPushMatrix();
  ofMultMatrix(lander_.getModelMatrix());
  ofNoFill();
  ofSetColor(ofColor::white);

  for (auto& box : lander_bounding_boxes_) {
    box.Draw();
  }

  ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::DrawLanderBounds() {
  ofSetColor(ofColor::white);
  lander_bounds_.Draw();
}

//--------------------------------------------------------------
void ofApp::DrawLanderCollisionBoxes() {
  ofPushMatrix();
  ofMultMatrix(lander_.getModelMatrix());
  ofNoFill();
  ofSetColor(ofColor::lightBlue);

  for (const auto& box : lander_collision_boxes_) {
    box.Draw();
  }

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
    case 'B':
    case 'b':
      lander_bounding_boxes_displayed_ = !lander_bounding_boxes_displayed_;
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
    case 'S':
    case 's':
      SavePicture();
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
    case 'W':
    case 'w':
      ToggleWireframeMode();
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
void ofApp::SavePicture() {
  ofImage picture;
  picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());

  if (picture.save("screenshot.png")) {
    cout << "screenshot saved" << endl;
  } else {
    cerr << "screenshot not saved" << endl;
  }
}

//--------------------------------------------------------------
void ofApp::TogglePointsDisplay() {
  terrain_points_displayed_ = !terrain_points_displayed_;
}

//--------------------------------------------------------------
void ofApp::ToggleWireframeMode() { wireframe_enabled_ = !wireframe_enabled_; }

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
    auto lander_position{lander_.getPosition()};
    const auto mouse_position{
        GetMousePointOnPlane(lander_position, cam_.getZAxis())};
    const auto delta{mouse_position - mouse_last_pos_};

    lander_position += delta;

    lander_.setPosition(lander_position.x, lander_position.y,
                        lander_position.z);
    mouse_last_pos_ = mouse_position;

    const auto min_lander_bounds{lander_.getSceneMin() + lander_.getPosition()};
    const auto max_lander_bounds{lander_.getSceneMax() + lander_.getPosition()};
    const auto new_lander_bounds{Box(min_lander_bounds, max_lander_bounds)};

    lander_bounds_ = new_lander_bounds;

    lander_collision_boxes_.clear();
    octree_.Intersect(lander_bounds_, octree_.root_, lander_collision_boxes_);
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
  const auto origin{cam_.getPosition()};
  const auto mouse_world_space{
      cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0))};
  const auto mouse_direction{glm::normalize(mouse_world_space - origin)};
  float distance;

  const auto hit{glm::intersectRayPlane(origin, mouse_direction, plane_origin,
                                        plane_normal, distance)};

  if (hit) {
    // find the point of intersection on the plane. compute using parametric
    // representation of a line: p' = p + s * dir;
    const auto intersection_point{origin + distance * mouse_direction};

    return intersection_point;
  } else
    return glm::vec3(0.0f);
}

//--------------------------------------------------------------
bool ofApp::SelectOctreeNode(glm::vec3& return_point) {
  const auto mouse{glm::vec3(mouseX, mouseY, 0.0f)};
  const auto ray_origin{cam_.screenToWorld(mouse)};
  const auto ray_direction{glm::normalize(ray_origin - cam_.getPosition())};
  const auto mouse_ray{Ray(ray_origin, ray_direction)};

  point_selected_ = octree_.Intersect(mouse_ray, octree_.root_, selected_node_);

  if (point_selected_) {
    return_point = octree_.mesh_.getVertex(selected_node_.points_[0]);
  }

  return point_selected_;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  if (cam_.getMouseInputEnabled()) return;

  if (lander_loaded_) {
    const auto origin{cam_.getPosition()};
    const auto mouse_world_space{
        cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0))};
    const auto mouse_direction{glm::normalize(mouse_world_space - origin)};

    const auto min_lander_bounds{lander_.getSceneMin() + lander_.getPosition()};
    const auto max_lander_bounds{lander_.getSceneMax() + lander_.getPosition()};
    const auto new_lander_bounds{Box(min_lander_bounds, max_lander_bounds)};

    lander_bounds_ = new_lander_bounds;

    const auto hit =
        lander_bounds_.Intersect(Ray(origin, mouse_direction), 0, 10000);

    if (hit) {
      lander_selected_ = true;
      terrain_selected_ = false;
      dragging_ = true;
      mouse_last_pos_ =
          GetMousePointOnPlane(lander_.getPosition(), cam_.getZAxis());
    } else {
      lander_selected_ = false;
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
void ofApp::dragEvent(ofDragInfo dragInfo) {
  // support drag-and-drop of model (.obj) file loading. when
  // model is dropped in viewport, place origin under cursor

  if (lander_.loadModel(dragInfo.files[0])) {
    lander_loaded_ = true;
    lander_.setPosition(0.0f, 0.0f, 0.0f);
    lander_.setScaleNormalization(false);

    cout << "number of meshes: " << lander_.getNumMeshes() << endl;

    lander_bounding_boxes_.clear();

    for (auto i = 0; i < lander_.getMeshCount(); i++) {
      lander_bounding_boxes_.push_back(
          Box::CreateMeshBoundingBox(lander_.getMesh(i)));
    }

    // goal: place model at mouse pointer
    // strategy: intersect a plane parallel to camera plane. find point of
    // intersection, then place lander there.

    // ray setup
    const auto origin{cam_.getPosition()};
    const auto mouse_world_space{
        cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0))};
    const auto mouse_direction{glm::normalize(mouse_world_space - origin)};
    const auto plane_origin{glm::vec3(0.0f)};
    const auto plane_normal{cam_.getZAxis()};
    float distance;

    glm::intersectRayPlane(origin, mouse_direction, plane_origin, plane_normal,
                           distance);

    // find the point of intersection on the plane. compute using parametric
    // representation of a line: p' = p + s * dir;
    const auto intersection_point{origin + distance * mouse_direction};

    const auto min_lander_bounds{lander_.getSceneMin()};
    const auto max_lander_bounds{lander_.getSceneMax()};
    lander_bounds_ = Box(min_lander_bounds, max_lander_bounds);

    const auto offset{(max_lander_bounds.y - min_lander_bounds.y) / 2.0f};
    lander_.setPosition(intersection_point.x, intersection_point.y - offset,
                        intersection_point.z);
  }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

////--------------------------------------------------------------
// bool ofApp::MouseIntersectPlane(const glm::vec3& plane_point,
//                                const glm::vec3& plane_normal,
//                                glm::vec3& intersection_point) {
//  const auto ray_origin{cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0.0f))};
//  const auto ray_direction{glm::normalize(ray_origin - cam_.getPosition())};
//
//  return Utility::RayIntersectPlane(ray_origin, ray_direction, plane_point,
//                                    plane_normal, intersection_point);
//}

//--------------------------------------------------------------
void ofApp::SetCameraTarget() {
  // Set the camera to use the selected point as it's new target
}
