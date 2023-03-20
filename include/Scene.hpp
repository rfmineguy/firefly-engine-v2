#ifndef SCENE_HPP
#define SCENE_HPP
#include "Entity.hpp"
#include "Components.hpp"
#include <string>
#include <vector>

namespace FF {
class Scene {
public:
  Scene();
  ~Scene();
  Entity* NewEntity(const std::string&);
  void DeleteEntity(Entity*&);

  // TODO: What to do when the entity doesn't exist
  Entity FindEntity(const std::string&);
  void Traverse();
  
  Entity* FindEntityNode(const std::string&);
  int GetEntityCount() const;

public:
  void DeserializeFromFile(const std::string&);
  void SerializeToFile(const std::string&);
  
private:
  void InsertEntity(Entity*, Entity&);
  void InsertEntity(Entity*, Entity&, Entity);
  Entity FindEntityRec(Entity*, const std::string&);
  Entity* FindEntityNodeRec(Entity*, const std::string&);
  void TraverseRec(Entity*, int);
  void Clean(Entity*);

private:
  void SerializeEntitiesRec(nlohmann::json&, Entity*);
  void SerializeTreeRec(nlohmann::json&, Entity*);
  void DeserializeEntities(nlohmann::json);
  void DeserializeTreeRec(nlohmann::json, Entity*, int = 0);

private:
  Entity *entity_tree_root, *selected_entity;
  int entity_count;
  std::shared_ptr<entt::registry> registry;  // for the reference counting
  
public:
  friend class ImGuiHeirarchyPane;  //allow heirarchy pane to have access to the private members
  friend class ImGuiInspectorPane;  //allow Inspector pane to have access to the private members
  friend class ImGuiViewportPane;   //allow Viewport pane to have access to the private members
};
}

#endif