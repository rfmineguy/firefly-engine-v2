#ifndef ENTITY_HP
#define ENTITY_HP
#include <entt-src/single_include/entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>
#include <vector>

#define DIRTY_RETURN \
if (is_dirty) \
  return

namespace FF {
class Scene;    // foward declare scene
class Entity {
public:
  // static Entity* Deserialize(YAML::Node, FF::Scene&, int);
  // static YAML::Node Serialize(Entity*);

  static Entity* Deserialize(nlohmann::json, FF::Scene&, int);
  static nlohmann::json Serialize(Entity*);
  
public:
  Entity();
  Entity(std::shared_ptr<entt::registry>);
  Entity(const std::string&, std::shared_ptr<entt::registry>);
  ~Entity();
  
  const std::string& GetName() const;
  Entity* AddChild(Entity*);
  Entity* RemoveChild(Entity*);
  void MarkDirty(bool);
  
private:
  void MarkDirtyRec(Entity*, bool);
  
public:
  bool operator==(const Entity&) const;
  bool operator!=(const Entity&) const;

// 
public:
  template<typename T>
  T* GetComponent() const {
    if (is_dirty || registry_ptr.expired())
      return nullptr;
    if (!this->HasComponent<T>()) {
      std::cerr << "Entity('" << name << "') has no component '" << typeid(T).name() << "'" << std::endl;
      return nullptr;
    }
    return &registry_ptr.lock()->get<T>(handle);
  }
  
  template <typename T, typename ...Args>
  void AddComponent(Args&&... args) {
    if (is_dirty || registry_ptr.expired())
      return;
    if (HasComponent<T>()) {
      std::cerr << "Entity('" << name << "') already has component '" << typeid(T).name() << "'" << std::endl;
      return;
    }
    registry_ptr.lock()->emplace<T>(handle, std::forward<Args>(args)...);
  }
  
  template <typename T>
  bool HasComponent() const {
    if (is_dirty || registry_ptr.expired())
      return false;
    return registry_ptr.lock()->try_get<T>(handle);
  }  

public:
  static int entity_count;

private:
  bool is_dirty;
  entt::entity handle;
  std::string name;
  Entity* parent;
  std::vector<Entity*> children;
  
private:
  std::weak_ptr<entt::registry> registry_ptr;
friend class Scene;
friend class ImGuiHeirarchyPane;
friend class ImGuiViewportPane;
friend class ImGuiInspectorPane;
};
}

#endif