#ifndef PROJECT_HPP
#define PROJECT_HPP
#include <string>

namespace FF {
class Project {
public:
  Project();
  ~Project();
  void Load(const std::string& path);

  void Serialize();
  void Deserialize();
};
}

#endif