#ifndef BIRD_HPP
#define BIRD_HPP
#include "vector.hpp"
#include "primitives.hpp"
#include "../utils/calculation-functions.cpp"
#include "constants.hpp"
#include <GL/gl.h>

enum WING_ANGLE_TYPE { POSITIVE, NEGATIVE };
class Bird {
private:
  bool isDeleted = false;
  vec3 pos;
  vec3 beginPos;
  float wingAngle = 25;
  char currentWingAngleType = WING_ANGLE_TYPE::POSITIVE;
  char yt ='p';
  char xt = 'p';
  char zt = 'p';
  float angleBodyForward = 0;
  float angleBodySide = 0;

public:
  Bird() : pos(vec3(0.0, 0.0, 0.0)) {}
  Bird(vec3 pos_) : pos(pos_), beginPos(pos_) {}

  void changeWingAngle(float angle_change_rate,  float speed);
  void drawBird(Colors colors, Primitives primitives);
  void setRadomPosition(float speed);
  void upBirdAngleBodyForward(float value);
  void setAngleBodySide(float value);

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

#endif