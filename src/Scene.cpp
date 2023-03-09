#include "../include/Scene.hpp"
#include <iostream>
#include <memory>

namespace FF {

Scene::Scene() {
  registry = std::make_shared<entt::registry>();
  entity_tree_root = new Entity("root", registry);
  entity_tree_root->AddComponent<Identifier>("root");
  std::cout << "Created scene tree" << std::endl;
}

Scene::~Scene() {
  Clean(entity_tree_root);
  delete entity_tree_root;
  entity_tree_root = nullptr;
  std::cout << "Deleted scene tree" << std::endl;
}

void Scene::Clean(Entity* root) {
  if (root == nullptr) {
    return;
  }
  for (int i = 0; i < root->children.size(); i++) {
    Clean(root->children[i]);
    std::cout << root->children[i] << std::endl;
    delete root->children[i];
    root->children[i] = nullptr;
  }
}

Entity* Scene::NewEntity(const std::string& name) {
  Entity *e = new Entity(name, registry);
  e->AddComponent<Identifier>(name);
  entity_tree_root->AddChild(e);
  return e;
}

void Scene::DeleteEntity(Entity e) {

}

Entity Scene::FindEntity(const std::string& id) {
  return FindEntityRec(entity_tree_root, id);
}

void Scene::Traverse() {
  std::cout << "Traversing scene" << std::endl;
  TraverseRec(entity_tree_root, 0);
}

void Scene::TraverseRec(Entity* root, int level) {
  if (root == nullptr)
    return;
  for (int i = 0; i < level; i++) {
    std::cout << "  ";
  }
  std::cout << "\\_";
  std::cout << root->GetComponent<Identifier>().id << std::endl;
  for (int i = 0; i < root->children.size(); i++) {
    TraverseRec(root->children[i], level+1);
  }
}

Entity* Scene::FindEntityNode(const std::string& id) {
  std::cout << "Finding entity: " << id << std::endl;
  return FindEntityNodeRec(entity_tree_root, id);
}

// Use depth or breadth first search
//  Probably depth first search
//  https://www.youtube.com/watch?v=TtAflDtqwVg
//  https://en.wikipedia.org/wiki/Depth-first_search
Entity* Scene::FindEntityNodeRec(Entity* root, const std::string& id) {
  Entity* found = nullptr;
  if (root == nullptr)
    return nullptr;
  if (root->GetComponent<Identifier>().id == id)
    return root;

  if (root->children.size() == 0)
    return nullptr;

  for (int i = 0; i < root->children.size(); i++) {
    found = FindEntityNodeRec(root->children.at(i), id);
    if (found != nullptr) {
      break;
    }
  }
  return found;
}

Entity Scene::FindEntityRec(Entity* root, const std::string& id) {
  Entity* found = FindEntityNodeRec(root, id);
  if (found == nullptr)
    throw std::string("No entity found");
  return *found;
}

void Scene::InsertEntity(Entity* root, Entity& e) {
  InsertEntity(entity_tree_root, e, *root);
}

void Scene::InsertEntity(Entity* root, Entity& e, Entity parent) {
  if (root == nullptr)
    return;
  if (*root == parent) {
    std::cout << "Found the parent" << std::endl;
    root->children.push_back(new Entity(e));
    return;
  }
  for (int i = 0; i < root->children.size(); i++) {
    InsertEntity(root->children[i], e, parent);
  }
}
}
