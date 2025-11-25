#include "bird.hpp"
#include <iostream>

void Bird::changeWingAngle(float angle_change_rate, float speed) {

  if (currentWingAngleType == POSITIVE)
    wingAngle += angle_change_rate * speed;
  else
    wingAngle -= angle_change_rate * speed;

  if (wingAngle >= MAX_ANGLE)
    currentWingAngleType = NEGATIVE;
  if (wingAngle <= -MAX_ANGLE)
    currentWingAngleType = POSITIVE;
}

void Bird::drawBird(Colors colors, Primitives primitives) {
  // asa direita
  glPushMatrix();
  glRotatef(angleBodyForward + (wingAngle / 200 * 100), 1.0, 0.0, 0);
  glRotatef(angleBodySide, 0, 0.0, 1);
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glRotatef(90, 0.0, 1.0, 0);
  glRotatef(90, 0.0, 0.0, 1.0);
  glRotatef(90, 1.0, 0.0, 0.0);
  glRotatef(wingAngle, 0.0, 0.0, 1.0);
  primitives.desenharPiramide(0.5, 1.5, 3, colors);
  glPopMatrix();

  // asa esquerda
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glRotatef(90, 0.0, 1.0, 0);
  glRotatef(90, 0.0, 0.0, 1.0);
  glRotatef(-90, 1.0, 0.0, 0.0);
  glRotatef(wingAngle, 0.0, 0, 1);
  primitives.desenharPiramide(0.5, 1.5, 3, colors);
  glPopMatrix();

  // bico
  glPushMatrix();
  glTranslatef(0, 0, 1);
  glRotatef(90, 0.0, 1.0, 0);
  glRotatef(90, 0.0, 0.0, 1.0);
  primitives.desenharPiramide(1.5, 1.5, 2, colors);
  glPopMatrix();

  // rabo
  glPushMatrix();
  glTranslatef(0, 0, -2.5);
  glRotatef(90, 0.0, 1.0, 0);
  glRotatef(90, 0.0, 0.0, 1.0);
  primitives.desenharPiramide(0.5, 0.5, 2, colors);
  glPopMatrix();

  // corpo
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glRotatef(90, 0.0, 1.0, 0.0);
  primitives.desenhaParalelepipedo(2, 1, 1, colors);
  glPopMatrix();
  glPopMatrix();
}

void Bird::setRadomPosition(float speed) {

  float driftX = ((rand() % 100) / 100.0f - 0.5f) * 0.02f;
  float driftY = ((rand() % 100) / 100.0f - 0.5f) * 0.02f;
  float driftZ = ((rand() % 100) / 100.0f - 0.5f) * 0.02f;

  if (yt == 'p')
    pos.y += (0.1 + driftY) * speed;
  else
    pos.y -= (0.1 + driftY) * speed;

  if (xt == 'p')
    pos.x += (0.1 + driftX) * speed;
  else
    pos.x -= (0.1 + driftX) * speed;

  if (zt == 'p')
    pos.z += (0.1 + driftZ) * speed;
  else
    pos.z -= (0.11 + driftZ) * speed;

  if (pos.y < -10)
    yt = 'p';
  if (pos.y > 40)
    yt = 'n';

  if (pos.x < -60)
    xt = 'p';
  if (pos.x > 60)
    xt = 'n';

  if (pos.z < -80)
    zt = 'p';
  if (pos.z > 0)
    zt = 'n';
}

void Bird::upBirdAngleBodyForward(float value) { 
  if(value == 0 && angleBodyForward < 0) {
    angleBodyForward += 1;
  } else {
    angleBodyForward = value;
  }
  if(angleBodyForward > 0) angleBodyForward = 0;
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

void Bird::setAngleBodySide(float value) {
  angleBodySide = value;
}