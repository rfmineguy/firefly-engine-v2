#ifndef ENTITY_HP
#define ENTITY_HP
#include <entt-src/single_include/entt/entt.hpp>
#include <iostream>
#include <memory>

namespace FF {
class Entity {
public:
  static Entity InvalidEntity();

private:
  Entity();

public:
  Entity(std::shared_ptr<entt::registry>);
  Entity(const std::string&, std::shared_ptr<entt::registry>);
  ~Entity();
  bool operator==(const Entity&) const;
  bool operator!=(const Entity&) const;

  template<typename T>
  T& GetComponent() {
    if (!HasComponent<T>()) {
      std::cerr << "Entity('" << name << "') has no component '" << typeid(T).name() << "'" << std::endl;
    }
    return registry_ptr.lock()->get<T>(handle);
  }
  
  template <typename T, typename ...Args>
  void AddComponent(Args&&... args) {
    registry_ptr.lock()->emplace<T>(handle, std::forward<Args>(args)...);
  }
  
  template <typename T>
  bool HasComponent() {
    return registry_ptr.lock()->try_get<T>(handle);//try_get<T>(handle);
  }
  
  const std::string& GetName() const;
  
  friend void SetRegistry(std::shared_ptr<entt::registry>, Entity*);

public:
  static int entity_count;

private:
  entt::entity handle;
  std::string name;
  bool is_valid;
  
private:
  std::weak_ptr<entt::registry> registry_ptr;
};
}

#endif