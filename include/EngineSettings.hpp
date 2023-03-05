#ifndef ENGINE_SETTINGS_HPP
#define ENGINE_SETTINGS_HPP
#include <glm/glm.hpp>

namespace FF_Int {
class EngineSettings {
public:
  static EngineSettings& Get() {
    static EngineSettings instance;
    return instance;
  }
};
}
#endif