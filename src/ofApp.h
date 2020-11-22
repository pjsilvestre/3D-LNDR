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
  void setup();
  void initLightingAndMaterials();

  void update();

  void draw();
  void drawAxis(ofVec3f);

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

  bool bAltKeyDown;
  bool bCtrlKeyDown;
  bool bInDrag = false;
  bool bWireframe;
  bool bDisplayPoints;
  bool bPointSelected;
  bool bHide;
  bool pointSelected = false;
  bool bDisplayLeafNodes = false;
  bool bDisplayOctree = false;
  bool bDisplayBBoxes = false;

  bool bLanderLoaded;
  bool bLanderSelected = false;
  bool bTerrainSelected;

  const float selectionRange = 4.0;

  ofEasyCam cam;
  ofxAssimpModelLoader mars;
  ofxAssimpModelLoader lander;
  ofLight light;

  ofxIntSlider numLevels;
  ofxPanel gui;

  ofVec3f selectedPoint;
  ofVec3f intersectPoint;
  glm::vec3 mouseDownPos;
  glm::vec3 mouseLastPos;

  Box boundingBox;
  Box landerBounds;
  Box testBox;
  vector<Box> colBoxList;
  vector<Box> bboxList;

  TreeNode selectedNode;
  Octree octree;
};
