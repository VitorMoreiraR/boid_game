#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "camera.hpp"
#include "colors.hpp"
#include "constants.hpp"
#include "object.hpp"
#include "primitives.hpp"
#include "bird.hpp"
#include "boid.hpp"
#include "../utils/calculation-functions.cpp"

std::vector<Object> obstacles;
bool pause = false;
Camera camera(vec3(0, 30, 0));
Boid* boid = new Boid();
Colors colors = Colors();
Primitives primitives = Primitives();

void resize(int w, int h) {
  glViewport(0, 0, w, h);

  float aspect = (float)w / (float)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, aspect, 0.1, 500.0);

  glMatrixMode(GL_MODELVIEW);
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

  boid->keysAddRemoveBirds(window, key, action);
}


void drawParallelepipeds() {
  for (int i = 0; i < 10; i++) {
    glPushMatrix();
    glTranslatef(obstacles[i].pos.x, obstacles[i].pos.y, obstacles[i].pos.z);
    primitives.desenhaParalelepipedo(10, 30, 10, colors);
    glPopMatrix();
  }
}

void draw(GLFWwindow *window) {
  float rad = boid->getBoidRotationAngleY() * (3.14159265358979323846f / 180.0f);
  vec3 seep_vector(sin(rad), 0.0f, cos(rad));

  camera.cameraLogic(seep_vector, boid->getLead());

  glPushMatrix();
  glPushMatrix();
  glTranslatef(0, 15, 0.0); // cone
  primitives.desenhaCone(30.0, 10.0, 10, 20, colors);
  glPopMatrix();
  primitives.desenhaTerreno(1, colors);
  glPopMatrix();

  glPushMatrix();
  glColor3f(1, 0, 0);
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 5);
  glEnd();
  glPopMatrix();

  drawParallelepipeds();
  boid->drawBoid(rad, pause, obstacles, colors, primitives);
}

void init() {

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

    boid->includeBoidMovement(window, pause);

   boid->changeSpeedAndAngleChangeRate(window, pause);
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}