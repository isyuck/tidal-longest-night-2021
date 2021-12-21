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
  ofEnableAlphaBlending();
  ofSetWindowTitle("longest-tidal");
  // camera.enableOrtho();
  fbo.allocate(1920, 1080, GL_RGB4);
  tex.allocate(1920, 1080, GL_RGBA);

  fbo.begin();
  ofClear(255, 255, 255, 0);
  fbo.end();

  ofPlanePrimitive plane(512, 320, 16, 16);
  mesh = plane.getMesh();

  textures.resize(153);
  for (int i = 0; i < 153; i++) {
    const std::string path = "bgs/img" + std::to_string(i) + ".png";
    ofLoadImage(textures.at(i), path);
    std::cout << "main loaded background image from path " << path << "\n";
  }

  // create & setup the tracks
  for (int i = 0; i < TRACK_COUNT; i++) {
    tracks.push_back(Track(static_cast<TrackNumber>(i)));
  }
  for (auto &t : tracks) {
    t.setup();
  }
}

void ofApp::update() {

  doClear = false;

  if (anyhit > 0) {
    anyhit--;
  }
  if (delta > 0.0f) {
    delta -= 0.5f;
  }

  for (auto &t : tracks) {
    t.update();
  }

  while (osc.hasWaitingMessages()) {
    ofxOscMessage msg;
    osc.getNextMessage(&msg);

    try {
      TrackNumber midichan = NONE;
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
        delta = MAX_DELTA;
        currentImageIndex = ofRandom(0, textures.size() - 1);
        std::cout << "received OSC for track " << midichan << '\n';
        if (midichan < TRACK_COUNT) {
          tracks.at(midichan).hit();
        }
        if (midichan == BD or midichan == BT) {
          doClear = true;
        }
      }

    } catch (...) {
      std::cerr << "ERROR: unhandled OSC!\n";
    }
  }

  float range = 1 - (delta / 100.0f);
  backgroundColor = ofColor(ofLerp(backgroundColor.r, 0, range),
                            ofLerp(backgroundColor.g, 0, range),
                            ofLerp(backgroundColor.b, 0, range)

  );

  for (int i = 0; i < mesh.getNumVertices(); i++) {
    auto v = mesh.getVertex(i);
    v.z = ofLerp(v.z, 0.0f, 1 - (delta / MAX_DELTA));
    mesh.setVertex(i, v);
  }

  for (int i = 0; i < mesh.getNumVertices(); i++) {
    auto v = mesh.getVertex(i);
    float p = (i * 0.1) + (ofGetElapsedTimeMillis() * 0.0005f);
    v.x += (ofNoise(p) - 0.5f) * (2.0f * (1.1 - (delta / MAX_DELTA)));
    v.y += (ofNoise(p + 10) - 0.5f) * (2.0f * (1.1 - (delta / MAX_DELTA)));
    v.z += (ofNoise(p + 15) - 0.5f) * (2.0f * (1.1 - (delta / MAX_DELTA)));
    mesh.setVertex(i, v);
  }
}

void ofApp::fboDraw() {

  if (anyhit == 4) {
    // ofClear(255, 255, 255, 200);
    for (int i = 0; i < mesh.getNumVertices(); i++) {
      auto v = mesh.getVertex(i);
      v.z += ofRandom(-30, 30);
      // v.z = ofRandom(-10, 10);
      mesh.setVertex(i, v);
    }
  }

  if (doClear) {
    ofClear(255, 255, 255, 25);
  }

  ofSetBackgroundColor(0, 255, 0);
  ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

  ofPushMatrix();
  ofTranslate(ofGetWidth() / 4, ofGetHeight() / 4);
  ofRotateZDeg(180);
  ofRotateYDeg(180);
  ofScale(bgScale);
  ofSetColor(255, 255, 255, 100);
  textures.at(currentImageIndex).bind();
  mesh.draw();
  textures.at(currentImageIndex).unbind();
  ofPopMatrix();

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

void ofApp::draw() {
  fbo.begin();
  ofSetColor(255, 255, 255, 200);
  fboDraw();
  fbo.end();
  ofDisableBlendMode();

  ofScale(2, 2);
  tex = fbo.getTexture();
  tex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
  tex.draw(0, 0);
}

void ofApp::keyPressed(int key) {
  if (key == '=') {
    if (bgScale <= 20.0f) {
      bgScale += 0.2f;
    }
  } else if (key == '-') {
    if (bgScale >= 0.6f) {
      bgScale -= 0.2f;
    }
  }
}
