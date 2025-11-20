#pragma once
class Bird {
public:
  bool deleted = false;
  vec3 pos;
  vec3 beginPos;
  float wingAngle = 25;
  char wingAngleType = 'p';
  Bird() : pos(vec3(0.0, 0.0, 0.0)) {}
  Bird(vec3 pos_) : pos(pos_), beginPos(pos_) {}
};