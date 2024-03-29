#ifndef PERSPCAMERA_HPP
#define PERSPCAMERA_HPP
#include "Camera.hpp"

namespace FF {
class PerspectiveCamera: public Camera {
public:
  PerspectiveCamera();
  ~PerspectiveCamera();
  virtual void Update();
  virtual glm::mat4 GetView();
  virtual glm::mat4 GetProj();

  void UpdateCameraVectors();

private:
  glm::vec3 front, direction, worldUp, right, camUp;
  float yaw, pitch;
};
}

#endif