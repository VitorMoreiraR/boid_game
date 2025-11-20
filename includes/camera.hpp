#pragma once

#include "vector.hpp"
#include "bird.hpp"

enum CameraType {
    BEHIND_BOID,
    CENTER,
    NORMAL_SPEED_VECTOR,
    TOP
};

class Camera {

private:
 vec3 m_pos;
  vec3 m_dir;
  vec3 m_left;
  vec3 m_up;
  vec3 look;
  float m_yaw;
  float m_pitch = 0.0f;
  float static constexpr PI = 3.14159265;
  float cvtToRad(float ang);
  CameraType cameraCurrentType = CameraType::CENTER;

public:
  Camera(vec3 pos);
  ~Camera();
  void aenable(bool comDir);

  void changePosition(vec3 m_pos);
  void changeRotation(vec3 dir);
  void updateYaw(float dYaw);
  void updatePitch(float dPitch);
  void update();
  void setYaw(float yaw);
  void setPitch(float dPitch);
  void changeLookFor(vec3 look);
  void changeCameraType();
  void cameraLogic(vec3 speed_vector, Bird lead);
};
