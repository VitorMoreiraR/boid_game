#include "camera.hpp"
#include <GL/glu.h>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

Camera::Camera(vec3 pos) {

  m_pos = vec3(pos);
  m_dir = vec3(0, 0, -1);
  m_left = vec3(-1, 0, 0);
  m_up = vec3(0, 1, 0);
  look = m_pos + m_dir;
}

Camera::~Camera() {}

void Camera::aenable(bool comDir) {
  vec3 currentLook = look;
  if (comDir)
    currentLook = m_pos + m_dir;
  gluLookAt(m_pos.x, m_pos.y, m_pos.z, currentLook.x, currentLook.y,
            currentLook.z, m_up.x, m_up.y, m_up.z);
}

void Camera::changeLookFor(vec3 newLook) { look = newLook; }

void Camera::changePosition(vec3 pos) { m_pos = pos; }

void Camera::changeRotation(vec3 dir) { m_dir = dir; }

void Camera::updateYaw(float dYaw) { m_yaw += dYaw; }

void Camera::setYaw(float yaw) { m_yaw = yaw; }

void Camera::update() {
  float yawRad = cvtToRad(m_yaw);
  float pitchRad = cvtToRad(m_pitch);

  // fórmula completa para direção 3D
  m_dir.x = cos(pitchRad) * sin(yawRad);
  m_dir.y = sin(pitchRad);
  m_dir.z = -cos(pitchRad) * cos(yawRad);

  m_dir.normaliza();

  // recalcula os outros vetores
  m_left = m_up.prodVetorial(m_dir);
  m_left.normaliza();
}

void Camera::updatePitch(float dPitch) {
  m_pitch += dPitch;

  // evita virar completamente e inverter a câmera
  if (m_pitch > 89.0f)
    m_pitch = 89.0f;
  if (m_pitch < -89.0f)
    m_pitch = -89.0f;
}

void Camera::setPitch(float dPitch) { m_pitch = dPitch; }

float Camera::cvtToRad(float ang) { return (ang * PI) / 180.0; }

void Camera::changeCameraType() {

  switch (cameraCurrentType) {
  case CameraType::BEHIND_BOID: {
    cameraCurrentType = CameraType::CENTER;
    break;
  }

  case CameraType::CENTER: {
    // cameraCurrentType = CameraType::NORMAL_SPEED_VECTOR;
    cameraCurrentType = CameraType::TOP;
    break;
  }

  case CameraType::NORMAL_SPEED_VECTOR: {
    cameraCurrentType = CameraType::TOP;
    break;
  }

  case CameraType::TOP: {
    cameraCurrentType = CameraType::BEHIND_BOID;
    break;
  }

  default:
    break;
  }
}

void Camera::cameraLogic(vec3 speed_vector, Bird lead) {
  glLoadIdentity();

  switch (cameraCurrentType) {
  case CameraType::BEHIND_BOID: {
    float BEHIND_DISTANCE = 50.0f;
    float height = 7.0f;
    vec3 cameraPos = lead.pos - speed_vector * BEHIND_DISTANCE;
    cameraPos.y += height;
    this->setYaw(89.0f);
    this->changePosition(cameraPos);
    this->changeLookFor(lead.pos);
    this->update();
    this->aenable(false);
    break;
  }
  case CameraType::CENTER: {
    vec3 up(0.0, 1.0, 0.0);
    this->changePosition(vec3(0, 30, 0));
    this->changeLookFor(lead.pos);
    this->aenable(false);
    break;
  }
  case CameraType::NORMAL_SPEED_VECTOR: {
    break;
  }
  case CameraType::TOP: {
    this->changeRotation(vec3(0, 0, -1));
    this->setPitch(-89.0f);
    this->setYaw(0);
    this->changePosition(
        vec3(lead.pos.x + speed_vector.x, 200, lead.pos.z + speed_vector.z));
    this->update();
    this->aenable(true);
    break;
  }
  default:
    break;
  }
}
