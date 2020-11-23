/**
 * @class ofApp
 * @brief Application main entry point
 * @author Kevin M. Smith (CS 134 SJSU)
 * @author Patrick Silvestre
 */

//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
//
//
//  Student Name: Patrick Silvestre
//  Date: 11/22/2020

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
  void InitializeLighting();

  void update() override;

  void draw() override;
  void DrawWireframes();
  void DrawAxis(const glm::vec3& location);
  void DrawLanderBoundingBoxes();
  void DrawLanderBounds();
  void DrawLanderCollisionBoxes();
  void DrawTerrainPoints();
  void DrawPointSelected();
  void DrawOctree();

  void keyPressed(int key) override;
  void SavePicture();
  void TogglePointsDisplay();
  void ToggleWireframeMode();
  void ToggleSelectTerrain();

  void keyReleased(int key) override;

  void mouseMoved(int x, int y) override;

  void mouseDragged(int x, int y, int button) override;
  glm::vec3 GetMousePointOnPlane(const glm::vec3& plane_origin,
                                 const glm::vec3& plane_normal);
  bool SelectOctreeNode(glm::vec3& return_point);

  void mousePressed(int x, int y, int button) override;

  void mouseReleased(int x, int y, int button) override;

  void mouseEntered(int x, int y) override;
  void mouseExited(int x, int y) override;
  void windowResized(int w, int h) override;

  void dragEvent(ofDragInfo dragInfo) override;

  void gotMessage(ofMessage msg) override;

  bool MouseIntersectPlane(const glm::vec3& plane_point,
                           const glm::vec3& plane_normal,
                           glm::vec3& intersection_point);
  void SetCameraTarget();

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

  glm::vec3 mouse_last_pos_;

  Box lander_bounds_;
  vector<Box> lander_bounding_boxes_;
  vector<Box> terrain_collision_boxes_;

  TreeNode selected_node_;
  Octree octree_;
};
