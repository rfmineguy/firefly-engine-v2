#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm-src/glm/glm.hpp>

namespace FF {
class Camera {
public:
  struct MatrixStack {
    glm::mat4 view;
    glm::mat4 projection;
  };

public:
  Camera();
  Camera(glm::vec3);
  ~Camera();
  virtual void Update() = 0;
  virtual glm::mat4 GetView() = 0;
  virtual glm::mat4 GetProj() = 0;
  virtual void SetProjSize(float, float);
  void DeltaPos(float, float);

protected:
  float width, height;
  glm::vec3 position;
  MatrixStack mvp;
};
}

#endif