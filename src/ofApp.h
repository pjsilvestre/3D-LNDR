/**
 * @class ofApp
 * @brief Application main entry point
 * @author Kevin M. Smith (CS 134 SJSU)
 * @author Patrick Silvestre
 *
 * TODO fuel-based thruster limitation
 * TODO landing area win condition
 * TODO lander physics (explosion upon impact velocity upon some threshold)
 * TODO fix shaders
 */

#pragma once

#include "glm/gtx/intersect.hpp"
#include "lander-system.h"
#include "octree.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include "particle-emitter.h"

class ofApp : public ofBaseApp {
 public:
  void setup() override;
  void LoadAssets();
  void SetUpCameras();
  void SetUpLighting();

  void update() override;
  void UpdateCameras();
  void UpdateLighting();

  void draw() override;
  // void SetUpVertexBuffer();
  void DrawAltimeterGauge() const;
  void DrawAxis(const glm::vec3& location) const;
  void DrawControlHints() const;

  void keyPressed(int key) override;
  void StartThrusterEffects();

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

  bool dragging_ = false;
  bool terrain_selected_ = true;
  bool gui_displayed_ = false;
  bool shaders_loaded_ = false;

  ofCamera* current_cam_ = &follow_cam_;
  ofCamera follow_cam_;
  ofCamera onboard_cam_;
  ofCamera tracking_cam_;
  ofEasyCam free_cam_;

  ofImage background_;

  ofLight landing_area_light_;
  ofLight terrain_light_;
  ofLight thruster_light_;

  ofSoundPlayer thrust_sound_player_;

  ofTrueTypeFont altimeter_font_;
  ofTrueTypeFont control_hint_font_;

  ofxAssimpModelLoader mars_;

  // ofTexture particle_texture_;
  // ofShader shader_;
  // ofVbo vertex_buffer_;

  glm::vec3 landing_area_ = glm::vec3(-5.0f, -10.0f, 40.0f);
  glm::vec3 mouse_last_pos_ = glm::vec3(0.0f);

  Octree octree_;
  LanderSystem lander_system_;
  ParticleEmitter thruster_;
};
