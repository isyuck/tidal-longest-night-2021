#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 57122

enum TrackNumber {
  BD = 0,
  SD = 1,
  RS = 2,
  CP = 3,
  BT = 4,
  LT = 5,
  MT = 6,
  HT = 7,
  CH = 8,
  OH = 9,
  CY = 10,
  CB = 11,
  NONE = 12,
};

struct Track {
  int n;
  // this is not the same as tidal's delta
  float delta;
  // true if we received a message this frame
  bool hit;
};

class ofApp : public ofBaseApp {

public:
  void setup();
  void update();
  void draw();
  ofxOscReceiver osc;
  Track tracks[12];
  ofMesh mesh;
};
