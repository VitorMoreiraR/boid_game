#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include "camera.hpp"
#include "colors.hpp"
#include "constants.hpp"
#include "object.hpp"
#include "primitives.hpp"
#include "bird.hpp"

float angle_change_rate = 0.15f;
int current_number_of_birds = 0;
std::vector<Bird> birds;
std::vector<Object> obstacles;
Bird lead(vec3(10, 15, -10));
float boidRotationAngleY = 0;
float speed = 0.1f;
char cameraType = 'c';
bool pause = false;

Camera camera(vec3(0, 30, 0));
float length(vec3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

vec3 computeVerticalRepulsionForce(const Bird &bird, const Object &obstacle) {

  vec3 obstacle_center = obstacle.pos;
  vec3 bird_pos = bird.pos + lead.pos;
  vec3 diff_xz = obstacle_center;
  diff_xz.y = 0.0f;
  bird_pos.y = 0.0f;

  vec3 vector_xz = diff_xz - bird_pos;
  float distance_xz = length(vector_xz);
  if (distance_xz > SAFETY_RADIUS_XZ) {
    return vec3(0.0f, 0.0f, 0.0f);
  }

  if ((bird.pos.y + lead.pos.y) > MAX_OBSTACLE_Y + BIRD_CLEARANCE_Y) {
    return vec3(0.0f, 0.0f, 0.0f);
  }

  float factor = (SAFETY_RADIUS_XZ - distance_xz) / SAFETY_RADIUS_XZ;

  float vertical_force = REPULSION_STRENGTH * speed * factor;

  float top_proximity =
      MAX_OBSTACLE_Y + BIRD_CLEARANCE_Y - (bird.pos.y + lead.pos.y);
  vertical_force *= std::min(1.0f, top_proximity / (MAX_OBSTACLE_Y * 2.0f));

  if (vertical_force < 0.0f)
    vertical_force = 0.0f;

  return vec3(0.0f, vertical_force, 0.0f);
}

int randomInt(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

void resize(int w, int h) {
  glViewport(0, 0, w, h);

  float aspect = (float)w / (float)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, aspect, 0.1, 500.0);

  glMatrixMode(GL_MODELVIEW);
}

vec3 getBirdPosition(int index) {

  float distance = 8.0f;
  float height = 0.0f;

  bool left = (index % 2 == 0);
  int level = (index / 2) + 1;

  float offsetZ = level * distance;
  float offsetX = (distance * level) * (left ? -1 : 1);

  return vec3(offsetX, height, -offsetZ);
}

int getFreeIndex() {
  std::vector<int> desactives;
  for (int i = 0; i < TOTAL_BIRDS; ++i) {
    if (birds[i].deleted)
      desactives.push_back(i);
  }
  if (desactives.empty())
    return -1;
  int k = randomInt(0, (int)desactives.size() - 1);
  return desactives[k];
}

int getUsedIndex() {
  std::vector<int> actives;
  for (int i = 0; i < TOTAL_BIRDS; ++i) {
    if (!birds[i].deleted)
      actives.push_back(i);
  }
  if (actives.empty())
    return -1;
  int k = randomInt(0, (int)actives.size() - 1);
  return actives[k];
}

void addBird() {

  int index = getFreeIndex();
  if (index == -1) {
    return;
  }
  vec3 posic = getBirdPosition(index);
  Bird obj(posic);
  obj.deleted = false;
  birds[index] = obj;
  current_number_of_birds += 1;
}

void removeBird() {
  if (current_number_of_birds == 0)
    return;
  int index = getUsedIndex();
  if (index == -1)
    return;
  birds[index].deleted = true;
  current_number_of_birds -= 1;
}

void keysAddRemoveBirds(GLFWwindow *window, int key, int action) {
  if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    addBird();
  } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    removeBird();
  }
}

void keys_callback(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (key == GLFW_KEY_C && action == GLFW_PRESS) {
    camera.changeCameraType();
  }

  if (key == GLFW_KEY_F && action == GLFW_PRESS && !pause) {
    if (glIsEnabled(GL_FOG)) {
      glDisable(GL_FOG);
    } else {
      glEnable(GL_FOG);
    }
  }

  if (key == GLFW_KEY_P && action == GLFW_PRESS) {
    pause = !pause;
  }

  keysAddRemoveBirds(window, key, action);
}

void changeWingAngle(Bird &obj) {
  if (obj.wingAngleType == 'p') {
    obj.wingAngle -= angle_change_rate;
  } else if (obj.wingAngleType == 'n') {
    obj.wingAngle += angle_change_rate;
  }

  if (obj.wingAngle > MAX_ANGLE) {
    obj.wingAngleType = 'p';
  } else if (obj.wingAngle < -1 * MAX_ANGLE) {
    obj.wingAngleType = 'n';
  }
}

void drawBoid(float rad) {
  glPushMatrix();

  glPushMatrix();
  glTranslatef(lead.pos.x, lead.pos.y, lead.pos.z);
  glRotatef(boidRotationAngleY, 0.0, 1.0, 0.0);
  // Desenha o líder
  desenhaPassarro(lead.wingAngle);

  if (!pause)
    changeWingAngle(lead);
  // Desenha seguidores
  vec3 formationOffset(0, 0, 0);
  for (int i = 0; i < TOTAL_BIRDS; i++) {
    if (birds[i].deleted)
      continue;
    vec3 total_repulsion_force(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < 11; j++) {
      total_repulsion_force =
          total_repulsion_force +
          computeVerticalRepulsionForce(birds[i], obstacles[j]);
    }
    if (total_repulsion_force.y > formationOffset.y)
      formationOffset = total_repulsion_force;

    if (formationOffset.y > 0.0 && !pause) {
      birds[i].pos = birds[i].pos + formationOffset;
    } else if (birds[i].pos.y > birds[i].beginPos.y && !pause) {
      const float RETURN_FORCE_FACTOR = 0.005f;
      float diff_y = birds[i].pos.y - birds[i].beginPos.y;
      float return_speed = diff_y * RETURN_FORCE_FACTOR;
      birds[i].pos.y -= return_speed;
      if (birds[i].pos.y < birds[i].beginPos.y) {
        birds[i].pos.y = birds[i].beginPos.y;
      }
    }
    glPushMatrix();
    glTranslatef(birds[i].pos.x, birds[i].pos.y, birds[i].pos.z);
    desenhaPassarro(birds[i].wingAngle);
    if (!pause)
      changeWingAngle(birds[i]);
    glPopMatrix();
  }
  glPopMatrix();
  glPopMatrix();

  if (pause)
    return;
  lead.pos.x += sin(rad) * speed;
  lead.pos.z += cos(rad) * speed;
}


void drawParallelepipeds() {
  for (int i = 0; i < 10; i++) {
    glPushMatrix();
    glTranslatef(obstacles[i].pos.x, obstacles[i].pos.y, obstacles[i].pos.z);
    desenhaParalelepipedo(10, 30, 10);
    glPopMatrix();
  }
}

void draw(GLFWwindow *window) {
  float rad = boidRotationAngleY * (3.14159265358979323846f / 180.0f);
  vec3 seep_vector(sin(rad), 0.0f, cos(rad));

  camera.cameraLogic(seep_vector, lead);

  glPushMatrix();
  glPushMatrix();
  glTranslatef(0, 15, 0.0); // cone
  desenhaCone(30.0, 10.0, 10, 20);
  glPopMatrix();
  desenhaTerreno(1);
  glPopMatrix();

  glPushMatrix();
  glColor3f(1, 0, 0);
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 5);
  glEnd();
  glPopMatrix();

  drawParallelepipeds();
  drawBoid(rad);
}

void moveBirds(int type) { boidRotationAngleY += type * 0.1f; }

void init() {

  for (int i = 0; i < TOTAL_BIRDS; i++) {
    vec3 posic = getBirdPosition(i);
    Bird obj(posic);
    obj.deleted = true;
    birds.push_back(obj);
  }

  for (int i = 0; i < 10; i++) {
    vec3 posic = vec3(randomInt(-200, 200), 15, randomInt(-200, 200));
    Object obj(posic);
    obstacles.push_back(obj);
  }
  vec3 posic = vec3(0, 15, 0.0);
  Object obj(posic);
  obstacles.push_back(obj);

  glClearColor(0.f, 0.f, 0.f, 1.f);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);

  float globalAmb[] = {0.1f, 0.1f, 0.1f, 1.f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

  float light0[4][4] = {
      {0.1f, 0.1f, 0.1f, 1.f}, // ambient
      {0.8f, 0.8f, 0.8f, 1.f}, // diffuse
      {1.f, 1.f, 1.f, 1.f},    // specular
      {0.f, 0.f, 1.f, 1.f}     // position
  };

  glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
  glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);
  glLightfv(GL_LIGHT0, GL_POSITION, &light0[3][0]);

  GLfloat fogColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_FOG);
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogi(GL_FOG_MODE, GL_EXP);
  glFogf(GL_FOG_DENSITY, 0.005f);
  glFogf(GL_FOG_START, 1.0f);
  glFogf(GL_FOG_END, 5.0f);
  glHint(GL_FOG_HINT, GL_NICEST);
}

int main(void) {

  camera.changeRotation(vec3(0, 0, -1));

  glfwInit();

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "THE BOID", NULL, NULL);

  glfwMakeContextCurrent(window);
  init();
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Erro ao inicializar GLEW!" << std::endl;
    return -1;
  }

  glfwSetKeyCallback(window, keys_callback);

  glClearColor(0.0, 0.15, 0.25, 1.0);
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {

    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    resize(width, height);

    draw(window);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      if (pause)
        continue;
      moveBirds(1);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      if (pause)
        continue;
      moveBirds(-1);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      if (pause)
        continue;

      if (speed < MAX_SPEED) {
        speed += 0.001f;
        angle_change_rate += 0.01f;
      };
      if (angle_change_rate < MAX_ANGLE_SPEED) {
        angle_change_rate += 0.01f;
      }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      if (pause)
        continue;
      if (speed > MIN_SPEED) {
        speed -= 0.001f;
      }
      if (angle_change_rate > MIN_ANGLE_SPEED) {
        angle_change_rate -= 0.01f;
      }
    }
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}