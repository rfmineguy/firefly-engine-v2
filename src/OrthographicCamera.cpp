#include "../include/OrthographicCamera.hpp"
#include <glm-src/glm/glm.hpp>
#include <glm-src/glm/gtc/matrix_transform.hpp>

namespace FF {
OrthoCamera::OrthoCamera() {
  
}

OrthoCamera::~OrthoCamera() {
  
}

void OrthoCamera::Update() {
  
}

glm::mat4 OrthoCamera::GetView() {
  glm::mat4 transform = glm::translate(glm::mat4(1.f), position);
  mvp.view = glm::inverse(transform);
  return mvp.view;
}

glm::mat4 OrthoCamera::GetProj() {
  mvp.projection = glm::ortho(0.f, width, height, 0.f, -1.f, 1.f);
  return mvp.projection;
}
}