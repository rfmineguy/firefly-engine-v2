#ifndef ENTITY_HP
#define ENTITY_HP
#include <entt-src/single_include/entt/entt.hpp>

namespace FF {
class Entity {
public:
  static Entity InvalidEntity();

private:
  Entity();

public:
  Entity(const std::string&, entt::entity);
  Entity(entt::entity);
  ~Entity();
  bool operator==(const Entity&) const;
  bool operator!=(const Entity&) const;

  template<typename T>
  T& GetComponent();
  
  template <typename T>
  void AddComponent();
  
  template <typename T>
  bool HasComponent();
  
  const std::string& GetName() const;
  
private:
  entt::entity handle;
  std::string name;
  bool is_valid;
};
}

#endif