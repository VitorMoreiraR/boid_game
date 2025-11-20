#include "bird.hpp"
#include "constants.hpp"

void Bird::changeWingAngle(float angle_change_rate) {

  if (currentWingAngleType == POSITIVE)
    wingAngle += angle_change_rate;
  else
    wingAngle -= angle_change_rate;

  if (wingAngle >= MAX_ANGLE)
    currentWingAngleType = NEGATIVE;
  if (wingAngle <= -MAX_ANGLE)
    currentWingAngleType = POSITIVE;
}

bool Bird::getIsDeleted() const { return isDeleted; }

vec3 Bird::getPos() const { return pos; }

vec3 Bird::getBeginPos() const { return beginPos; }

float Bird::getWingAngle() const { return wingAngle; }

char Bird::getCurrentWingAngleType() const { return currentWingAngleType; }

void Bird::setPos(const vec3 &newPos) { pos = newPos; }

void Bird::setIsDeleted(bool deleted) { isDeleted = deleted; }

void Bird::setWingAngle(float angle) { wingAngle = angle; }

void Bird::setCurrentWingAngleType(char type) { currentWingAngleType = type; }