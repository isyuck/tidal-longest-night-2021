#pragma once

#include "TrackNumber.h"
#include "ofMain.h"

#ifndef TRACK_H
#define TRACK_H

#define MAX_DELTA 100
#define IMG_PER_TRACK 10
#define IMG_PLANE_RES 32

class Track {
public:
  Track(TrackNumber number);
  void setup();
  void update();
  void draw();
  void hit();
  TrackNumber number;

private:
  // frames since last hit
  float delta = 0.0f;
  int currentImageIndex = 0;
  std::vector<ofTexture> textures;
  ofMesh mesh;
  ofVec3f desiredRot = ofVec3f();
  ofVec3f actualRot = ofVec3f();
  ofVec3f rotationEuler = ofVec3f(0, 0, 0);
  std::vector<float> actualzs;
  float desiredScale = 1.0f;
  float actualScale = 1.0f;
  float actualOpacity = 0.0f;
  float desiredOpacity = 0.0f;
  float initOff;
};

#endif
