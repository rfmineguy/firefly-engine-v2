#include "../include/Camera.hpp"

namespace FF {
Camera::Camera(): position(0), mvp{glm::mat4(1), glm::mat4(1)} {}
Camera::Camera(glm::vec3 pos): position(pos), mvp{glm::mat4(1), glm::mat4(1)} {}
Camera::~Camera() {}
void Camera::SetProjSize(float _width, float _height) {
  width = _width;
  height = _height;
}
void Camera::DeltaPos(float dx, float dy) {
  position.x += dx;
  position.y += dy;
}
}