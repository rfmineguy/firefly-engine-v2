#ifndef SCENE
#define SCENE
#include <entt-src/src/entt/entt.hpp>
#include <vector>

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

private:
  void TraverseRec(entt::entity, int = 0);

public:
  template<typename T>
  T* GetComponent(entt::entity);
  template <typename T>
  bool HasComponent(entt::entity);
  template <typename T, typename ...Args>
  void AddComponent(entt::entity, Args&&... args);

private:
  entt::registry registry;
  entt::entity scene_root;

friend int main();
};
}

#endif