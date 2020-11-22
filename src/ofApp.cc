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
  ofPushMatrix();
  ofMultMatrix(lander_.getModelMatrix());
  ofNoFill();
  ofSetColor(ofColor::white);
  lander_bounds_.Draw();
  ofPopMatrix();
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
      setCameraTarget();
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
  // if moving camera, don't allow mouse interaction
  if (cam_.getMouseInputEnabled()) return;

  if (dragging_) {
    glm::vec3 landerPos = lander_.getPosition();

    glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam_.getZAxis());
    glm::vec3 delta = mousePos - mouse_last_pos_;

    landerPos += delta;
    lander_.setPosition(landerPos.x, landerPos.y, landerPos.z);
    mouse_last_pos_ = mousePos;

    ofVec3f min = lander_.getSceneMin() + lander_.getPosition();
    ofVec3f max = lander_.getSceneMax() + lander_.getPosition();

    Box bounds =
        Box(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, max.y, max.z));

    lander_collision_boxes_.clear();
    octree_.Intersect(bounds, octree_.root_, lander_collision_boxes_);
  } else {
    ofVec3f p;
    raySelectWithOctree(p);
  }
}

//--------------------------------------------------------------
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
  //  intersect the mouse ray with the plane normal to the camera
  //  return intersection point.   (package code above into function)

  // Setup our rays
  glm::vec3 origin = cam_.getPosition();
  glm::vec3 camAxis = cam_.getZAxis();
  glm::vec3 mouseWorld = cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0));
  glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
  float distance;

  bool hit =
      glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

  if (hit) {
    // find the point of intersection on the plane using the distance
    // We use the parametric line or vector representation of a line to compute
    //
    // p' = p + s * dir;
    //
    glm::vec3 intersectPoint = origin + distance * mouseDir;

    return intersectPoint;
  } else
    return glm::vec3(0, 0, 0);
}

//--------------------------------------------------------------
bool ofApp::raySelectWithOctree(ofVec3f& pointRet) {
  ofVec3f mouse(mouseX, mouseY);
  ofVec3f rayPoint = cam_.screenToWorld(mouse);
  ofVec3f rayDir = rayPoint - cam_.getPosition();
  rayDir.normalize();
  Ray ray = Ray(rayPoint, rayDir);

  point_selected_ = octree_.Intersect(ray, octree_.root_, selected_node_);

  if (point_selected_) {
    pointRet = octree_.mesh_.getVertex(selected_node_.points_[0]);
  }

  return point_selected_;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  // if moving camera, don't allow mouse interaction
  if (cam_.getMouseInputEnabled()) return;

  // if rover is loaded, test for selection
  if (lander_loaded_) {
    glm::vec3 origin = cam_.getPosition();
    glm::vec3 mouseWorld = cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0));
    glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

    ofVec3f min = lander_.getSceneMin() + lander_.getPosition();
    ofVec3f max = lander_.getSceneMax() + lander_.getPosition();

    Box bounds =
        Box(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, max.y, max.z));
    bool hit = bounds.Intersect(Ray(origin, mouseDir), 0, 10000);

    if (hit) {
      lander_selected_ = true;
      terrain_selected_ = false;
      mouse_down_pos_ =
          getMousePointOnPlane(lander_.getPosition(), cam_.getZAxis());
      mouse_last_pos_ = mouse_down_pos_;
      dragging_ = true;
    } else {
      lander_selected_ = false;
      terrain_selected_ = true;
    }
  } else {
    ofVec3f p;
    raySelectWithOctree(p);
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
  // support drag-and-drop of model (.obj) file loading.  when
  // model is dropped in viewport, place origin under cursor

  if (lander_.loadModel(dragInfo.files[0])) {
    lander_loaded_ = true;
    lander_.setScaleNormalization(false);
    lander_.setPosition(0, 0, 0);
    cout << "number of meshes: " << lander_.getNumMeshes() << endl;
    lander_bounding_boxes_.clear();

    for (int i = 0; i < lander_.getMeshCount(); i++) {
      lander_bounding_boxes_.push_back(
          Box::CreateMeshBoundingBox(lander_.getMesh(i)));
    }

    // lander_.setRotation(1, 180, 1, 0, 0);

    // We want to drag and drop a 3D object in space so that the model appears
    // under the mouse pointer where you drop it !
    //
    // Our strategy: intersect a plane parallel to the camera plane where the
    // mouse drops the model once we find the point of intersection, we can
    // position the lander/lander at that location.
    //

    // Setup our rays
    glm::vec3 origin = cam_.getPosition();
    glm::vec3 camAxis = cam_.getZAxis();
    glm::vec3 mouseWorld = cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0));
    glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
    float distance;

    bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0),
                                      camAxis, distance);
    if (hit) {
      // find the point of intersection on the plane using the distance
      // We use the parametric line or vector representation of a line to
      // compute
      //
      // p' = p + s * dir;
      //
      glm::vec3 intersectPoint = origin + distance * mouseDir;

      // Now position the lander's origin at that intersection point
      glm::vec3 min = lander_.getSceneMin();
      glm::vec3 max = lander_.getSceneMax();
      float offset = (max.y - min.y) / 2.0;
      lander_.setPosition(intersectPoint.x, intersectPoint.y - offset,
                          intersectPoint.z);

      // set up bounding box for lander while we are at it
      lander_bounds_ =
          Box(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, max.y, max.z));
    }
  }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm,
                                glm::vec3& point) {
  ofVec2f mouse(mouseX, mouseY);
  ofVec3f rayPoint = cam_.screenToWorld(glm::vec3(mouseX, mouseY, 0));
  ofVec3f rayDir = rayPoint - cam_.getPosition();
  rayDir.normalize();

  return (RayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
void ofApp::setCameraTarget() {
  // Set the camera to use the selected point as it's new target
}
