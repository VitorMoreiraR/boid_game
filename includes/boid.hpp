#ifndef BOID_HPP
#define BOID_HPP

#include "bird.hpp"
#include "constants.hpp"
#include "object.hpp"
#include "primitives.hpp"
#include "../utils/calculation-functions.cpp"
#include <GLFW/glfw3.h>
#include <vector>

class Boid {

public:
  Boid();
  void addBird();
  void removeBird();
  void keysAddRemoveBirds(GLFWwindow *window, int key, int action);
  void includeBoidMovement(GLFWwindow *window, bool pause);
  void drawBoid(float rad, bool pause, std::vector<Object> obstacles, Colors colors,  Primitives primitives);
  void changeSpeedAndAngleChangeRate(GLFWwindow *window, bool pause);

  Bird *getLead() const;
  const std::vector<Bird> &getBirds() const;
  float getSpeed() const;
  float getAngleChangeRate() const;
  int getCurrentNumberOfBirds() const;
  float getBoidRotationAngleY() const;

  void setLead(Bird *newLead);
  void setBirds(const std::vector<Bird> &newBirds);
  void setSpeed(float newSpeed);
  void setAngleChangeRate(float newRate);
  void setCurrentNumberOfBirds(int newCount);
  void setBoidRotationAngleY(float newAngle);

private:
  int getFreeIndex();
  int getUsedIndex();
  vec3 changeBirdPosition(int index);
  vec3 computeVerticalRepulsionForce(const Bird &bird, const Object &obstacle);
  void moveBirds(int type);

private:
  Bird *lead;
  std::vector<Bird> birds;
  float speed;
  float angle_change_rate;
  int current_number_of_birds;
  float boidRotationAngleY;
};

#endif
