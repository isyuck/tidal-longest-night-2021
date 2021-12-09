#pragma once

#include "Track.h"
#include "TrackNumber.h"
#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 57122
#define IMG_COUNT 53
#define TRACK_COUNT 4

class ofApp : public ofBaseApp {

public:
  void setup();
  void update();
  void draw();
  ofxOscReceiver osc;
  ofEasyCam camera;

  std::vector<Track> tracks;
  // frames since any hit
  int anyhit;
};
