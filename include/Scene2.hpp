#ifndef SCENE
#define SCENE
#include <entt-src/src/entt/entt.hpp>
#include <vector>
#include "../include/Logger.hpp"

namespace FF {
class Scene2 {
public:
  Scene2();
  ~Scene2();

public:
  entt::entity NewEntity(const std::string&, entt::entity);
  entt::entity NewEntity(const std::string&);
  
  entt::entity FindEntityById(const std::string&);
  void MoveEntity(entt::entity, entt::entity);
  void Traverse();

  bool IsChildOfRec(entt::entity, entt::entity);

private:
  void TraverseRec(entt::entity, int = 0);

public:
  template<typename T>
  T* GetComponent(entt::entity e) {
    if (e == entt::null) {
      FF_LOG_ERROR("GetComponent, entt::null");
    }
    if (!HasComponent<T>(e)) {
      return nullptr;
    }
    return &registry.get<T>(e);
  }

  template <typename T>
  bool HasComponent(entt::entity e)
  {
    return registry.try_get<T>(e);
  }
  
  template <typename T, typename ...Args>
  void AddComponent(entt::entity e, Args&&... args){
    registry.emplace<T>(e, std::forward<Args>(args)...);
  }

public:
  static int entity_count;

private:
  entt::registry registry;
  entt::entity scene_root;
  entt::entity selection;
friend int main();
friend class ImGuiHeirarchyPane;
friend class ImGuiViewportPane;
friend class ImGuiInspectorPane;
};
}

#endif