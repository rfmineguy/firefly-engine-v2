#include "../include/Scene.hpp"
#include <iostream>
#include <memory>

namespace FF {

Scene::Scene() {
  registry = std::make_shared<entt::registry>();
  entity_tree = new Node(registry); //this is the root
  // SetRegistry(&registry, &entity_tree->entity);
  entity_tree->entity.AddComponent<Identifier>("root");
  std::cout << "Created scene tree" << std::endl;
}

Scene::~Scene() {
  Clean(entity_tree);
  delete entity_tree;
  entity_tree = nullptr;
  std::cout << "Deleted scene tree" << std::endl;
}

void Scene::Clean(Node* root) {
  if (root == nullptr) {
    return;
  }
  for (int i = 0; i < root->children.size(); i++) {
    Clean(root->children[i]);
    delete root->children[i];
    root->children[i] = nullptr;
  }
}

// void SetRegistry(entt::registry* reg, Entity* entity) {
//   entity->registry_ptr = reg;
// }

Entity Scene::NewEntity(const std::string& name) {
  Entity e(name, registry);
  // SetRegistry(&registry, &e);
  e.AddComponent<Identifier>(name);
  InsertEntity(entity_tree, e);
  return e;
}

Entity Scene::NewEntity(const std::string& name, const std::string& parent) {
  Node* p = FindEntityNodeRec(entity_tree, parent);
  if (p == nullptr) {
    throw std::string("Parent not found");
  }
  std::cout << "Parent Found : " << p << std::endl;
  Entity e(name, registry);
  // SetRegistry(&registry, &e);
  e.AddComponent<Identifier>(name);
  Node* n = new Node(e);
  std::cout << n << std::endl;
  p->AddChild(new Node(e));
  // InsertEntity(entity_tree, e, p->entity);
  return e;
}

void Scene::DeleteEntity(Entity e) {

}

Entity Scene::FindEntity(const std::string& id) {
  return FindEntityRec(entity_tree, id);
}

void Scene::Traverse() {
  std::cout << "Traversing scene" << std::endl;
  TraverseRec(entity_tree, 0);
}

void Scene::TraverseRec(Node* root, int level) {
  if (root == nullptr)
    return;
  for (int i = 0; i < level; i++) {
    std::cout << "  ";
  }
  std::cout << "\\_";
  std::cout << root->entity.GetComponent<Identifier>().id << std::endl;
  for (int i = 0; i < root->children.size(); i++) {
    TraverseRec(root->children[i], level+1);
  }
}

Scene::Node* Scene::FindEntityNodeRec(Scene::Node* root, const std::string& id) {
  if (root == nullptr)
    return nullptr;
  if (root->entity.GetComponent<Identifier>().id == id)
    return root;
  for (int i = 0; i < root->children.size(); i++) {
    return FindEntityNodeRec(root->children.at(i), id);
  }
}

Entity Scene::FindEntityRec(Node* root, const std::string& id) {
  Node* found = FindEntityNodeRec(root, id);
  if (found == nullptr)
    throw std::string("No entity found");
  return found->entity;
}

void Scene::InsertEntity(Node* root, Entity& e) {
  InsertEntity(root, e, root->entity);
}

void Scene::InsertEntity(Node* root, Entity& e, Entity parent) {
  if (root == nullptr)
    return;
  if (root->entity == parent) {
    std::cout << "Found the parent" << std::endl;
    root->children.push_back(new Node(e));
    // SetRegistry(&registry, &e);
    return;
  }
  for (int i = 0; i < root->children.size(); i++) {
    InsertEntity(root->children[i], e, parent);
  }
}
}
