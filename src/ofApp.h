/**
 * @class ofApp
 * @brief Application main entry point
 * @author Kevin M. Smith (CS 134 SJSU)
 * @author Patrick Silvestre
 *
 * TODO lander physics (gravity, turbulence)
 * TODO lander control (rotation along up axis)
 * TODO lander exhaust using particle emitters and shaders
 * TODO altitude detection using ray intersection
 * TODO collision detection with terrain
 * TODO terrain lighting with 3+ lights, lander lighting with 1+ lights
 * TODO cameras (lander tracking camera, lander onboard camera, easycam)
 * TODO sound (thrust)
 * TODO background starfield
 */

#pragma once

#include "glm/gtx/intersect.hpp"
#include "lander.h"
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
  void DrawAxis(const glm::vec3& location);
  void DrawTerrainPoints();
  void DrawPointSelected();
  void DrawOctree();

  void keyPressed(int key) override;
  void TogglePointsDisplay();
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

  bool terrain_selected_{true};
  bool point_selected_{false};

  bool gui_displayed_{true};
  bool terrain_points_displayed_{false};
  bool leaf_nodes_displayed_{false};
  bool octree_displayed_{false};

  ofEasyCam cam_;
  ofxAssimpModelLoader mars_;
  Lander lander_;
  ofLight light_;

  ofxIntSlider num_octree_levels_;
  ofxPanel gui_;

  glm::vec3 mouse_last_pos_;

  TreeNode selected_node_;
  Octree octree_;
};
