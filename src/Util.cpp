#include <glm-src/glm/glm.hpp>
#include <iostream>
#include "../include/Util.hpp"

namespace FF {

void Util::DisplayVec3(glm::vec3 vec) {
  std::cout << "vec3 { x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z << " }" << std::endl;
}

}