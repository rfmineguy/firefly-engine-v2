#include "../include/PerspectiveCamera.hpp"
#include <glm-src/glm/gtc/matrix_transform.hpp>

namespace FF {
const float YAW = -90.f;
const float PITCH = 0.f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.f;

PerspectiveCamera::PerspectiveCamera()
  :Camera(glm::vec3(0, 0, 3)), 
  direction(0),
  front(0, 0, -1),
  worldUp(0),
  right(0), 
  camUp(0, 1, 0), yaw(0), pitch(0) {
  UpdateCameraVectors();
}

PerspectiveCamera::~PerspectiveCamera() {
  
}

void PerspectiveCamera::Update() {
  UpdateCameraVectors();
}

glm::mat4 PerspectiveCamera::GetView() {
  mvp.view = glm::lookAt(position, position + front, camUp);
  return mvp.view;
}

glm::mat4 PerspectiveCamera::GetProj() {
  mvp.projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, 0.1f, 100.f);
  return mvp.projection;
}

void PerspectiveCamera::UpdateCameraVectors() {
    // calculate the new Front vector
  glm::vec3 front_loc = glm::vec3(0);
  front_loc.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front_loc.y = sin(glm::radians(pitch));
  front_loc.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front_loc = glm::normalize(front_loc);
  // also re-calculate the Right and Up vector
  right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  camUp = glm::normalize(glm::cross(right, front));
}
}
