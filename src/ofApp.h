#pragma once

#include "Track.h"
#include "TrackNumber.h"
#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 57122
#define IMG_COUNT 53
#define TRACK_COUNT 12
#define MAX_DELTA 100

class ofApp : public ofBaseApp {

public:
  void setup();
  void update();
  void fboDraw();
  void draw();
  void keyPressed(int key);

  ofxOscReceiver osc;
  ofEasyCam camera;

  std::vector<Track> tracks;
  // frames since any hit (max 4)
  int anyhit;
  float delta = 0.0f;
  float bgScale = 1.0f;
  ofColor backgroundColor = ofColor(0, 0, 0);
  std::vector<ofTexture> textures;
  std::size_t currentImageIndex = 0;
  ofMesh mesh;
  ofFbo fbo;
  ofTexture tex;
  bool doClear = false;
};
