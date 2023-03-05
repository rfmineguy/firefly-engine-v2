#include "../include/Scene.hpp"
#include <iostream>

namespace FF {

Scene::Scene() {
  entity_tree = new Node(&registry); //this is the root
  entity_tree->entity.AddComponent<Identifier>("root");
}

Scene::~Scene() {
  Clean(entity_tree);
  delete entity_tree;
  entity_tree = nullptr;
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

Entity Scene::NewEntity(const std::string& name) {
  Entity e(registry.create());
  InsertEntity(entity_tree, e);
  return e;
}

Entity Scene::NewEntity(const std::string& name, const std::string& parent) {
  
}

void Scene::DeleteEntity(Entity e) {

}

Entity Scene::FindEntity(const std::string& name) {
  return FindEntityRec(entity_tree, name);
}

void Scene::Traverse() {
  std::cout << "Traversing scene" << std::endl;
  TraverseRec(entity_tree, 0);
}

void Scene::TraverseRec(Node* root, int level) {
  if (root == nullptr)
    return;
  for (int i = 0; i < level; i++) {
    std::cout << '\t';
  }
  std::cout << root->entity.GetName() << std::endl;
  for (int i = 0; i < root->children.size(); i++) {
    TraverseRec(root->children[i], level+1);
  }
}

Entity Scene::FindEntityRec(Node* root, const std::string& name) {
  if (root == nullptr) {
    throw std::string("Failed to find entity");
  }
  if (root->entity.GetName() == name) {
    return root->entity;
  }
  for (int i = 0; i < root->children.size(); i++) {
    return FindEntityRec(root->children[i], name);
  }
  return Entity::InvalidEntity();
}

void Scene::InsertEntity(Node* root, Entity e) {
  InsertEntity(root, e, root->entity);
}

void Scene::InsertEntity(Node* root, Entity e, Entity parent) {
  if (root == nullptr)
    return;
  if (root->entity == parent) {
    std::cout << "Found the parent" << std::endl;
    root->children.push_back(new Node(e));
    return;
  }
  for (int i = 0; i < root->children.size(); i++) {
    InsertEntity(root->children[i], e, parent);
  }
}
}
