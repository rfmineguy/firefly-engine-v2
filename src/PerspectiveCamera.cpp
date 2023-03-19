#include "../include/PerspectiveCamera.hpp"
#include <glm-src/glm/gtc/matrix_transform.hpp>

namespace FF {
PerspectiveCamera::PerspectiveCamera()
  :Camera(glm::vec3(0, 0, 0)), 
  target(0),
  direction(0),
  front(0, 0, -1),
  worldUp(0),
  right(0), 
  camUp(1), yaw(0), pitch(0) {
  UpdateCameraVectors();
}

PerspectiveCamera::~PerspectiveCamera() {
  
}

void PerspectiveCamera::Update() {
  UpdateCameraVectors();
}

void PerspectiveCamera::SetProjSize(float w, float h) {
  mvp.projection = glm::perspective(glm::radians(45.f), w / h, 0.1f, 100.f);
}

glm::mat4 PerspectiveCamera::GetView() {
  mvp.view = glm::lookAt(position, position + front, camUp);
  return mvp.view;
}

glm::mat4 PerspectiveCamera::GetProj() {
  return mvp.projection;
}

void PerspectiveCamera::UpdateCameraVectors() {
    // calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);
  // also re-calculate the Right and Up vector
  right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  camUp = glm::normalize(glm::cross(right, front));
}
}
