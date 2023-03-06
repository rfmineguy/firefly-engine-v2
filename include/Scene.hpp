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

    Node(entt::registry* reg)
      : entity(reg->create()) {}
    Node(Entity e)
      : entity(e) {}
    
    void AddChild(Node* n) {
      children.push_back(n);
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
  entt::registry registry;
  
public:
  friend class ImGuiHeirarchyPane;  //allow heirarchy pane to have access to the private members
};
}

#endif