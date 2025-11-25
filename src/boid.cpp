
#include "boid.hpp"

Boid::Boid() {
  angle_change_rate = 4;
  current_number_of_birds = 0;
  lead = new Bird(vec3(10, 15, -10));
  speed = 0.1f;
  boidRotationAngleY = 0;

  for (int i = 0; i < TOTAL_BIRDS; i++) {
    Bird obj(vec3(0, 0, 0));
    obj.setIsDeleted(true);
    birds.push_back(obj);
  }
}

void Boid::addBird() {

  int index = this->getFreeIndex();
  if (index == -1) {
    return;
  }
  vec3 position = changeBirdPosition(index);
  Bird bird(position);
  bird.setPos(vec3(randomFloat(-30, 30), randomFloat(-15, 15), randomFloat(-30, 0)));
  birds[index].setIsDeleted(false);
  birds[index] = bird;
  current_number_of_birds += 1;
}

void Boid::removeBird() {
  if (current_number_of_birds == 0)
    return;
  int index = getUsedIndex();
  if (index == -1)
    return;
  birds[index].setIsDeleted(true);
  current_number_of_birds -= 1;
}

vec3 Boid::computeVerticalRepulsionForce(const Bird &bird,
                                         const Object &obstacle) {

  vec3 obstacle_center = obstacle.pos;
  vec3 bird_pos = bird.getPos() + lead->getPos();
  vec3 diff_xz = obstacle_center;
  diff_xz.y = 0.0f;
  bird_pos.y = 0.0f;

  vec3 vector_xz = diff_xz - bird_pos;
  float distance_xz = vector_xz.length();
  if (distance_xz > SAFETY_RADIUS_XZ) {
    return vec3(0.0f, 0.0f, 0.0f);
  }

  if ((bird.getPos().y + lead->getPos().y) > MAX_OBSTACLE_Y + BIRD_CLEARANCE_Y) {
    return vec3(0.0f, 0.0f, 0.0f);
  }

  float factor = (SAFETY_RADIUS_XZ - distance_xz) / SAFETY_RADIUS_XZ;

  float vertical_force = REPULSION_STRENGTH * speed * factor;

  float top_proximity =
      MAX_OBSTACLE_Y + BIRD_CLEARANCE_Y - (bird.getPos().y + lead->getPos().y);
  vertical_force *= std::min(1.0f, top_proximity / (MAX_OBSTACLE_Y * 2.0f));

  if (vertical_force < 0.0f)
    vertical_force = 0.0f;

  return vec3(0.0f, vertical_force, 0.0f);
}

void Boid::keysAddRemoveBirds(GLFWwindow *window, int key, int action) {
  if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    addBird();
  } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    removeBird();
  }
}

void Boid::drawBoid(float rad, bool pause, std::vector<Object> obstacles, Colors colors, Primitives primitives) {
  glPushMatrix();

  glPushMatrix();
  glTranslatef(lead->getPos().x, lead->getPos().y, lead->getPos().z);
  glRotatef(boidRotationAngleY, 0.0, 1.0, 0.0);
  // Desenha o líder
  lead->drawBird(colors, primitives);

  if (!pause)
    lead->changeWingAngle(angle_change_rate, speed);
  // Desenha seguidores
 
  for (int i = 0; i < TOTAL_BIRDS; i++) {
    if (birds[i].getIsDeleted())
      continue;
    vec3 total_repulsion_force(0.0f, 0.0f, 0.0f);
    vec3 formationOffset(0, 0, 0);
    for (int j = 0; j < 11; j++) {
      total_repulsion_force =
          total_repulsion_force +
          computeVerticalRepulsionForce(birds[i], obstacles[j]);
    }
    if (total_repulsion_force.y > formationOffset.y)
      formationOffset = total_repulsion_force;
    if (formationOffset.y > 0.0 && !pause /*&&( birds[i].getPos().y + formationOffset.y) < 50*/) {
      if (fabs(formationOffset.y) < 0.001f) formationOffset.y = 0.0f;
      if (!pause){
         birds[i].setPos(birds[i].getPos() + formationOffset);
         birds[i].upBirdAngleBodyForward(-30);
        }
    } else {
      if (!pause) {
        birds[i].setRadomPosition(speed);
        birds[i].upBirdAngleBodyForward(0);
      };
    }
    glPushMatrix();
    glTranslatef(birds[i].getPos().x, birds[i].getPos().y, birds[i].getPos().z);
    birds[i].drawBird(colors, primitives);
    if (!pause)
      birds[i].changeWingAngle(angle_change_rate, speed);
    glPopMatrix();
     birds[i].setAngleBodySide(0);
  }
  glPopMatrix();
  glPopMatrix();

  if (pause)
    return;
  lead->setPos(vec3(lead->getPos().x + sin(rad) * speed, lead->getPos().y,
                    lead->getPos().z + cos(rad) * speed));
  lead->setAngleBodySide(0);
}

void Boid::includeBoidMovement(GLFWwindow *window, bool pause) {
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    if (!pause)
      this->moveBirds(-1);
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    if (!pause)
      this->moveBirds(1);
  }
}

int Boid::getFreeIndex() {
  std::vector<int> desactives;
  for (int i = 0; i < TOTAL_BIRDS; ++i) {
    if (birds[i].getIsDeleted())
      desactives.push_back(i);
  }
  if (desactives.empty())
    return -1;
  int k = randomInt(0, (int)desactives.size() - 1);
  return desactives[k];
}

int Boid::getUsedIndex() {
  std::vector<int> actives;
  for (int i = 0; i < TOTAL_BIRDS; ++i) {
    if (!birds[i].getIsDeleted())
      actives.push_back(i);
  }
  if (actives.empty())
    return -1;
  int k = randomInt(0, (int)actives.size() - 1);
  return actives[k];
}

vec3 Boid::changeBirdPosition(int index) {

  float distance = 8.0f;
  float height = 0.0f;

  bool left = (index % 2 == 0);
  int level = (index / 2) + 1;

  float offsetZ = level * distance;
  float offsetX = (distance * level) * (left ? -1 : 1);

  return vec3(offsetX, height, -offsetZ);
}

void Boid::changeSpeedAndAngleChangeRate(GLFWwindow *window, bool pause) {
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !pause) {
    if (speed < MAX_SPEED)
      speed += 0.001f;
    if (angle_change_rate < MAX_ANGLE_SPEED)
      angle_change_rate += 0.01f;
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !pause) {
    if (speed > MIN_SPEED)
      speed -= 0.001f;
    if (angle_change_rate > MIN_ANGLE_SPEED)
      angle_change_rate -= 0.01f;
  }
}

void Boid::moveBirds(int type) {{
   boidRotationAngleY += type * 1.2f * speed;
    for (int i = 0; i < TOTAL_BIRDS; i++) {
    if (birds[i].getIsDeleted()) continue;
      birds[i].setAngleBodySide(type * 20);
    }
    lead->setAngleBodySide(type * 20);
  } }

Bird *Boid::getLead() const { return lead; }

const std::vector<Bird> &Boid::getBirds() const { return birds; }

float Boid::getSpeed() const { return speed; }

float Boid::getAngleChangeRate() const { return angle_change_rate; }

int Boid::getCurrentNumberOfBirds() const { return current_number_of_birds; }

float Boid::getBoidRotationAngleY() const { return boidRotationAngleY; }

void Boid::setLead(Bird *newLead) { lead = newLead; }

void Boid::setBirds(const std::vector<Bird> &newBirds) { birds = newBirds; }

void Boid::setSpeed(float newSpeed) { speed = newSpeed; }

void Boid::setAngleChangeRate(float newRate) { angle_change_rate = newRate; }

void Boid::setCurrentNumberOfBirds(int newCount) {
  current_number_of_birds = newCount;
}

void Boid::setBoidRotationAngleY(float newAngle) {
  boidRotationAngleY = newAngle;
}