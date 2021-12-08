#include "ofApp.h"
#include "ofxOscArg.h"
#include "ofxOscMessage.h"
#include <algorithm>
#include <cstddef>
#include <utility>

void ofApp::setup() {
  osc.setup(PORT);
  const int height = 32;
  const int width = 32;
  for (int y = 0; y < height * 16; y += 16) {
    for (int x = 0; x < height * 16; x += 16) {
      mesh.addVertex(ofPoint(x, y, 0));       // make a new vertex
      mesh.addColor(ofFloatColor(0, 0, 255)); // add a color at that vertex
    }
  }
  for (int y = 0; y < height - 1; y++) {
    for (int x = 0; x < width - 1; x++) {
      mesh.addIndex(x + y * width);       // 0
      mesh.addIndex((x + 1) + y * width); // 1
      mesh.addIndex(x + (y + 1) * width); // 10

      mesh.addIndex((x + 1) + y * width);       // 1
      mesh.addIndex((x + 1) + (y + 1) * width); // 11
      mesh.addIndex(x + (y + 1) * width);       // 10
    }
  }
  mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
}

void ofApp::update() {

  for (int i = 0; i < 12; i++) {
    const auto tr = &tracks[static_cast<TrackNumber>(i)];
    tr->hit = false;
    if (tr->delta > 0.0) {
      tr->delta -= 0.005;
    }
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
        tracks[midichan].n = n;
        tracks[midichan].hit = true;
        tracks[midichan].delta = 1;
      }

    } catch (...) {
      std::cerr << "ERROR: unhandled OSC!\n";
    }
  }
}

void ofApp::draw() {
  if (tracks[BD].hit) {
    for (auto &v : mesh.getVertices()) {
      v.z = ofRandom(-50, 50);
    }
  }
  for (auto &v : mesh.getVertices()) {
    v.z = ofMap(tracks[BD].delta, 0, 1, 0, v.z);
  }

  std::cout << "[BD].delta: " << tracks[BD].delta << '\n';
  mesh.draw();
}
