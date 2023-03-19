#ifndef ORTHOCAMERA_HPP
#define ORTHOCAMERA_HPP
#include "Camera.hpp"

namespace FF {
class OrthoCamera: public Camera {
public:
  OrthoCamera();
  ~OrthoCamera();
  virtual void Update();
  virtual glm::mat4 GetView();
  virtual glm::mat4 GetProj();
};
}


#endif