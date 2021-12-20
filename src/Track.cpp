#include "Track.h"

Track::Track(TrackNumber number) : number(number) {}

void Track::setup() {
  initOff = ofRandom(0, 1);

  //--------------
  // ofPlanePrimitive plane(640, 640, IMG_PLANE_RES, IMG_PLANE_RES);
  // mesh = plane.getMesh();
  //--------------
  // ofSpherePrimitive sphere = ofSpherePrimitive(500, 32);
  // mesh = sphere.getMesh();
  //--------------
  ofBoxPrimitive box(500, 500, 500, 4,4,4);
  mesh = box.getMesh();

  actualzs.resize(mesh.getNumVertices());
  // load images into texture vector
  textures.resize(IMG_PER_TRACK);
  for (std::size_t i = 0; i < IMG_PER_TRACK; i++) {
    const std::size_t off = (static_cast<std::size_t>(number)) * IMG_PER_TRACK;
    const std::string path = "fgs/img" + std::to_string(off + i) + ".png";
    ofLoadImage(textures.at(i), path);
    std::cout << "track " << number << " loaded image from path " << path
              << '\n';
  }

  initTrans = ofVec3f(ofRandom(-100, 100),
                      ofRandom(-100, 100),
                      ofRandom(-100, 100));
}

void Track::update() {
  if (delta > 0) {
    delta -= 5;
  }

  auto alerp = [&](ofVec3f a, ofVec3f b, float x) {
    return ofVec3f(ofLerp(a.x, b.x, x), ofLerp(a.y, b.y, x),
                   ofLerp(a.z, b.z, x));
  };

  // for (int i = 0; i < actualzs.size(); i++) {
  //   auto v = mesh.getVertex(i);
  //   v.z = ofLerp(v.z, 200.0f, 1 - (delta / MAX_DELTA));
  //   mesh.setVertex(i, v);
  // }
  // actualOpacity = ofLerp(actualOpacity, desiredOpacity, 1 - (delta / 100));
  // color.a = ofLerp(color.a, 0.0f, 0.5 - (delta / 100.0f));
}

void Track::draw() {
  const auto &tex = textures.at(currentImageIndex + 1);
  // glRotatef(360.0f, actualRot.x, actualRot.y, actualRot.z);
  // color.r = ofLerp(color.r, 0.5f, 0.9 - (delta / MAX_DELTA));
  // color.g = ofLerp(color.g, 0.5f, 1.0 - (delta / MAX_DELTA));
  // color.b = ofLerp(color.b, 0.5f, 0.8 - (delta / MAX_DELTA));
  ofSetColor(255,255,255,ofRandom(0, 128));
  ofRotateRad(rotation.w + (ofGetElapsedTimeMillis() * 0.001), rotation.x, rotation.y, rotation.z);
  float s = ofMap(delta * scaleMult, 0, MAX_DELTA, 1, 5, false);
  ofScale(ofRandom(1, s));
  tex.bind();
  ofSetLineWidth(4);
  if (delta > 10) {
    mesh.draw();
    ofRotate(45);
    mesh.draw();
  } else if (delta > 5) {
    mesh.drawWireframe();
  }
  tex.unbind();

  for (int i = 0; i < mesh.getNumVertices(); i++) {
    auto v = mesh.getVertex(i);
    float p = (i + (initOff * 100) + (ofGetElapsedTimeMillis() * 0.01f));
    v.x += (ofNoise(p) - 0.5f) * (70.0f * (1 - (delta / MAX_DELTA)));
    v.y += (ofNoise(p) - 0.5f) * (70.0f * (1 - (delta / MAX_DELTA)));
    v.z += (ofNoise(p) - 0.5f) * (70.0f * (1 - (delta / MAX_DELTA)));
    mesh.setVertex(i, v);
  }
}

void Track::hit() {
  delta = MAX_DELTA;
  currentImageIndex = (int)ofRandom(0, IMG_PER_TRACK - 1);

  scaleMult = ofRandom(1, 5);

  actualScale = ofRandom(1, 15);
  desiredScale = ofRandom(1, 15);

  // random vector with magnitude between lo and hi
  auto randv = [&](float lo = 0.0f, float hi = 1.0f) {
    return ofVec3f(ofRandom(lo, hi), ofRandom(lo, hi), ofRandom(lo, hi));
  };

  actualRot = randv(-15, 15);
  desiredRot = randv(-15, 15);

  color.a = ofRandom(175, 255);
  color = ofColor(255, 255, 255);

  auto v = mesh.getVertex(0);
  // v.x += ofRandom(25);
  // v.y += ofRandom(25);
  // mesh.setVertex(0, v);

  // v = mesh.getVertex(mesh.getNumVertices() - 1);
  // v.x += ofRandom(25);
  // v.y += ofRandom(25);
  // mesh.setVertex(mesh.getNumVertices() - 1, v);

  rotation = ofVec4f(ofRandom(1), ofRandom(1), ofRandom(1), ofRandom(360));
  // for (int i = 0; i < actualzs.size(); i++) {
  //   v = mesh.getVertex(i);
  //   v.z += ofRandom(-0, 0);
  //   // v.z = ofRandom(-10, 10);
  //   mesh.setVertex(i, v);
  // }
}

void Track::setColor(float r, float g, float b, float a = 255) {
  color = ofColor(r, g, b, a);
}
