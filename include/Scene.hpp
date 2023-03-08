#ifndef SCENE_HPP
#define SCENE_HPP
#include "Entity.hpp"
#include "Components.hpp"
#include <string>
#include <vector>

namespace FF {
class Scene {
public:
  struct Node {
    Entity entity;
    Node* parent;
    std::vector<Node*> children;
    std::shared_ptr<entt::registry> reg_ptr;

    Node(std::shared_ptr<entt::registry> reg)
      : reg_ptr(reg), entity(reg) {
    }
    Node(Entity e)
      : entity(e) {}
    
    void AddChild(Node* n) {
      children.push_back(n);
    }
    
    friend void SetRegistry(std::shared_ptr<entt::registry> reg, Entity* e) {
      e->registry_ptr = reg;
    }
  };

public:
  Scene();
  ~Scene();
  Entity NewEntity(const std::string&);
  Entity NewEntity(const std::string&, const std::string&);
  void DeleteEntity(Entity);

  // TODO: What to do when the entity doesn't exist
  Entity FindEntity(const std::string&);
  void Traverse();
  
private:
  void InsertEntity(Node*, Entity&);
  void InsertEntity(Node*, Entity&, Entity);
  Node* FindEntityNodeRec(Node*, const std::string&);
  Entity FindEntityRec(Node*, const std::string&);
  void TraverseRec(Node*, int);
  void Clean(Node*);

private:
  Node* entity_tree;
  std::shared_ptr<entt::registry> registry;  // for the reference counting
  
public:
  friend class ImGuiHeirarchyPane;  //allow heirarchy pane to have access to the private members
};
}

#endif