//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
//
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>

#pragma once

#include "glm/gtx/intersect.hpp"
#include "octree.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include "utility.h"

class ofApp : public ofBaseApp {
 public:
  void setup() override;
  static void InitializeLighting();

  void update() override;

  void draw() override;
  void draw_wireframes();
  void draw_axis(const glm::vec3& location);
  void draw_lander_bounding_boxes();
  void draw_lander_bounds();
  void draw_lander_collision_boxes();
  void draw_terrain_points();
  void draw_point_selected();
  void draw_octree();

  void keyPressed(int key);
  void savePicture();
  void togglePointsDisplay();
  void toggleWireframeMode();
  // void toggleSelectTerrain();

  void keyReleased(int key);

  void mouseMoved(int x, int y);

  void mouseDragged(int x, int y, int button);
  glm::vec3 getMousePointOnPlane(glm::vec3 p, glm::vec3 n);
  bool raySelectWithOctree(ofVec3f& pointRet);

  void mousePressed(int x, int y, int button);

  void mouseReleased(int x, int y, int button);

  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);

  void dragEvent(ofDragInfo dragInfo);

  void gotMessage(ofMessage msg);

  bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm,
                           glm::vec3& point);
  void setCameraTarget();

  bool alt_key_down_{false};
  bool ctrl_key_down_{false};
  bool dragging_{false};

  bool lander_loaded_{false};
  bool lander_selected_{false};
  bool terrain_selected_{true};
  bool point_selected_{false};

  bool gui_displayed_{true};
  bool wireframe_enabled_{false};
  bool terrain_points_displayed_{false};
  bool leaf_nodes_displayed_{false};
  bool octree_displayed_{false};
  bool lander_bounding_boxes_displayed_{false};

  ofEasyCam cam_;
  ofxAssimpModelLoader mars_;
  ofxAssimpModelLoader lander_;
  ofLight light_;

  ofxIntSlider num_octree_levels_;
  ofxPanel gui_;

  ofVec3f selected_point_;
  ofVec3f intersect_point_;
  glm::vec3 mouse_down_pos_;
  glm::vec3 mouse_last_pos_;

  Box bounding_box_;
  Box lander_bounds_;
  vector<Box> lander_bounding_boxes_;
  vector<Box> lander_collision_boxes_;

  TreeNode selected_node_;
  Octree octree_;
};
