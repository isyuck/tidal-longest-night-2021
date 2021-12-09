#include "Track.h"

Track::Track(TrackNumber number) : number(number) {}

void Track::setup() {
  initOff = ofRandom(0, 1);

  // init mesh from plane primitive
  ofPlanePrimitive plane = ofPlanePrimitive();
  plane.setResolution(IMG_PLANE_RES, IMG_PLANE_RES);
  plane.setPosition(0, 0, 0);
  mesh = plane.getMesh();
  actualzs.resize(mesh.getNumVertices());

  // load images into texture vector
  textures.resize(IMG_PER_TRACK);
  for (std::size_t i = 0; i < IMG_PER_TRACK; i++) {
    const std::size_t off =
        (static_cast<std::size_t>(number) + 1) * IMG_PER_TRACK;
    const std::string path = "images/img" + std::to_string(off + i) + ".png";
    ofLoadImage(textures.at(i), path);
    std::cout << "track " << number << " loaded image from path " << path
              << '\n';
  }
}

void Track::update() {
  if (delta > 0) {
    delta -= 0.2;
  }

  auto alerp = [&](ofVec3f a, ofVec3f b, float x) {
    return ofVec3f(ofLerp(a.x, b.x, x), ofLerp(a.y, b.y, x),
                   ofLerp(a.z, b.z, x));
  };

  actualRot = alerp(actualRot, desiredRot, 1 - (delta / MAX_DELTA));

  for (int i = 0; i < actualzs.size(); i++) {
    auto v = mesh.getVertex(i);
    v.z = ofLerp(v.z, 0.0f, 1 - (delta / MAX_DELTA));
    mesh.setVertex(i, v);
  }
  actualScale = ofLerp(actualScale, desiredScale, 1 - (delta / 100));
  actualOpacity = ofLerp(actualOpacity, desiredOpacity, 1 - (delta / 100));
}

void Track::draw() {
  const auto &tex = textures.at(currentImageIndex + 1);
  // glRotatef(1.0f, actualRot.x, actualRot.y, actualRot.z);
  ofRotateXDeg(actualRot.x);
  ofRotateYDeg(actualRot.y);
  ofRotateZDeg(actualRot.z);
  ofRotateXDeg(sin((ofGetElapsedTimeMillis() + (initOff * 23487)) * 0.0005f) *
               360);
  ofScale(actualScale * ofRandom(0.8, 1.2), actualScale * ofRandom(0.5, 2));
  ofSetColor(255, 255, 255, actualOpacity);
  tex.bind();
  if (delta < 30) {
    mesh.drawWireframe();
  } else {
    mesh.draw();
  }
  tex.unbind();
}

void Track::hit() {
  delta = MAX_DELTA;
  currentImageIndex = (int)ofRandom(0, IMG_PER_TRACK - 1);

  actualScale = ofRandom(1, 15);
  desiredScale = ofRandom(1, 15);

  actualOpacity = ofRandom(0, 255);
  desiredOpacity = ofRandom(0, 255);

  // random vector with magnitude between lo and hi
  auto randv = [&](float lo = 0.0f, float hi = 1.0f) {
    return ofVec3f(ofRandom(lo, hi), ofRandom(lo, hi), ofRandom(lo, hi));
  };

  actualRot = randv(-15, 15);
  desiredRot = randv(-15, 15);

  auto v = mesh.getVertex(0);
  v.x += ofRandom(25);
  v.y += ofRandom(25);
  mesh.setVertex(0, v);

  v = mesh.getVertex(mesh.getNumVertices() - 1);
  v.x += ofRandom(25);
  v.y += ofRandom(25);
  mesh.setVertex(mesh.getNumVertices() - 1, v);

  rotationEuler = ofVec3f(ofRandom(1), ofRandom(1), ofRandom(1));
  for (int i = 0; i < actualzs.size(); i++) {
    if (ofRandom(100) < 5) {
      v = mesh.getVertex(i);
      v.z += ofRandom(-30, 30);
      // v.z = ofRandom(-10, 10);
      mesh.setVertex(i, v);
    }
  }
}
