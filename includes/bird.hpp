#pragma once

#include "vector.hpp"

enum WING_ANGLE_TYPE { POSITIVE, NEGATIVE };
class Bird {
private:
  bool isDeleted = false;
  vec3 pos;
  vec3 beginPos;
  float wingAngle = 25;
  char currentWingAngleType = WING_ANGLE_TYPE::POSITIVE;

public:
  Bird() : pos(vec3(0.0, 0.0, 0.0)) {}
  Bird(vec3 pos_) : pos(pos_), beginPos(pos_) {}

  void changeWingAngle(float angle_change_rate);

  bool getIsDeleted() const;
  vec3 getPos() const;
  vec3 getBeginPos() const;
  float getWingAngle() const;
  char getCurrentWingAngleType() const;

  void setPos(const vec3 &newPos);
  void setIsDeleted(bool deleted);
  void setWingAngle(float angle);
  void setCurrentWingAngleType(char type);
};