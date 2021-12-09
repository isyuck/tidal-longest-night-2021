#include "ofApp.h"
#include "ofxOscArg.h"
#include "ofxOscMessage.h"

void ofApp::setup() {
  osc.setup(PORT);
  // gfx
  ofSetBackgroundColor(0, 0, 0);
  ofDisableArbTex();
  ofDisableAntiAliasing();
  ofSetFrameRate(60);

  // create & setup the tracks
  for (int i = 0; i < TRACK_COUNT; i++) {
    tracks.push_back(Track(static_cast<TrackNumber>(i)));
  }
  for (auto &t : tracks) {
    t.setup();
  }
}

void ofApp::update() {

  if (anyhit > 0) {
    anyhit--;
  }

  for (auto &t : tracks) {
    t.update();
  }

  while (osc.hasWaitingMessages()) {
    ofxOscMessage msg;
    osc.getNextMessage(&msg);

    try {
      TrackNumber midichan = NONE;
      float delta = -1;
      int n = -1;

      for (std::size_t i = 0; i < msg.getNumArgs(); i++) {
        if (msg.getArgType(i) == OFXOSC_TYPE_STRING) {
          if ("midichan" == msg.getArgAsString(i)) {
            midichan = static_cast<TrackNumber>(msg.getArgAsFloat(i + 1));
          } else if ("n" == msg.getArgAsString(i)) {
            n = (int)msg.getArgAsFloat(i + 1);
          }
        }
      }

      if (midichan != NONE) {
        anyhit = 4;
        std::cout << "received OSC for track " << midichan << '\n';
        if (midichan < TRACK_COUNT) {
          tracks.at(midichan).hit();
        }
      }

    } catch (...) {
      std::cerr << "ERROR: unhandled OSC!\n";
    }
  }
}

void ofApp::draw() {

  ofSetBackgroundColor(0, 0, 0);
  switch (anyhit) {
  case 0:
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    break;
  case 1:
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    break;
  case 2:
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    break;
  case 3:
    ofSetBackgroundColor(0, 255, 0);
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    break;
  default:
    break;
  }

  camera.begin();
  // tracks.at(0).draw();
  for (auto &t : tracks) {
    ofPushMatrix();
    ofPushStyle();
    t.draw();
    ofPopMatrix();
    ofPopStyle();
  }
  camera.end();
}
