/**
 * @class ofApp
 * @brief Application main entry point
 * @author Kevin M. Smith (CS 134 SJSU)
 * @author Patrick Silvestre
 *
 * TODO lander physics (explosion upon impact velocity upon some threshold)
 * TODO lander exhaust using particle emitters and shaders
 * TODO sound (thrust)
 */

#pragma once

#include "glm/gtx/intersect.hpp"
#include "lander-system.h"
#include "octree.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
 public:
  void setup() override;
  void SetUpLighting();
  void SetUpCameras();

  void update() override;

  void draw() override;
  void DrawAltimeterGauge() const;
  void DrawAxis(const glm::vec3& location) const;
  void DrawControlHints() const;

  void keyPressed(int key) override;
  void keyReleased(int key) override;
  void mouseMoved(int x, int y) override;

  void mouseDragged(int x, int y, int button) override;
  glm::vec3 GetMousePointOnPlane(const glm::vec3& plane_origin,
                                 const glm::vec3& plane_normal) const;

  void mousePressed(int x, int y, int button) override;
  void mouseReleased(int x, int y, int button) override;
  void mouseEntered(int x, int y) override;
  void mouseExited(int x, int y) override;
  void windowResized(int w, int h) override;
  void dragEvent(ofDragInfo drag_info) override;
  void gotMessage(ofMessage msg) override;

  bool background_loaded_ = false;
  bool dragging_ = false;
  bool terrain_selected_ = true;
  bool gui_displayed_ = false;

  ofCamera* current_cam_ = &follow_cam_;
  ofCamera follow_cam_;
  ofCamera onboard_cam_;
  ofCamera tracking_cam_;
  ofEasyCam free_cam_;
  ofxPanel gui_;

  ofLight lander_point_light_;
  ofLight landing_area_point_light_;
  ofLight terrain_overhead_directional_light_;

  ofImage background_;
  ofxAssimpModelLoader mars_;

  glm::vec3 landing_area_ = glm::vec3(-5.0f, -10.0f, 40.0f);
  glm::vec3 mouse_last_pos_ = glm::vec3(0.0f);

  Octree octree_;
  LanderSystem lander_system_;
};
