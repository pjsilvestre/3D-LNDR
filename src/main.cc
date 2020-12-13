#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {
  // tested on 2560x1440
  ofSetupOpenGL(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                OF_GAME_MODE);

  ofRunApp(new ofApp());
}
