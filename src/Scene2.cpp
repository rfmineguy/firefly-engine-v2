#include "../include/Scene2.hpp"
#include "../include/Components.hpp"
#include <utility>
#include <iostream>
#include <optional>

namespace FF {
int Scene2::entity_count = 0;

Scene2::Scene2()
:scene_root(registry.create()) {
  AddComponent<Relationship>(scene_root);
  AddComponent<Identifier>(scene_root, "root");
  entity_count++;
  std::cout << "Scene2 constructor" << std::endl;
}

Scene2::~Scene2() {
  std::cout << "Scene2 destructor" << std::endl;
}

void Scene2::Traverse() {
  std::cout << "Traversing scene..." << std::endl;
  TraverseRec(scene_root);
}

// Is `this_entity` a child of `that_entity`
bool Scene2::IsChildOfRec(entt::entity this_entity, entt::entity that_entity) {
  Relationship* r = GetComponent<Relationship>(that_entity);
  if (this_entity == that_entity) {
    std::cout << "Found this_entity" << std::endl;
    return true;
  }
  for (int i = 0; i < r->children.size(); i++) {
    if (IsChildOfRec(this_entity, r->children.at(i))) {
      return true;
    }
  }
  return false;
}

void Scene2::TraverseRec(entt::entity root, int depth) {
  Relationship* r = GetComponent<Relationship>(root);      // NOTE: Assumes every entity has this Relationship component
  if (Identifier* id = GetComponent<Identifier>(root)) {
    for (int i = 0; i < depth; i++)
      std::cout << "   ";
    std::cout << "\\_" << id->id << std::endl;
  }
  if (r->children.size() == 0) {
    return;
  }
  for (int i = 0; i < r->children.size(); i++) {
    TraverseRec(r->children.at(i), depth + 1);
  }
}

entt::entity Scene2::NewEntity(const std::string& id, entt::entity parent) {
  auto view = registry.view<Identifier>();
  for (auto e : view) {
    if (GetComponent<Identifier>(e)->id == id) {
      std::cerr << "Entity with id [" << id << "] already exists" << std::endl;
      return entt::null;
    }
  }
  
  entt::entity e = registry.create();
  AddComponent<Identifier>(e, id + std::to_string(Scene2::entity_count));
  AddComponent<Relationship>(e);
  GetComponent<Relationship>(e)->parent = parent;
  GetComponent<Relationship>(parent)->children.push_back(e);
  AddComponent<Transform>(e);
  AddComponent<ShapeRenderer>(e);
  Scene2::entity_count++;
  return e;
}
entt::entity Scene2::NewEntity(const std::string& id) {
  return NewEntity(id, scene_root);
}

entt::entity Scene2::FindEntityById(const std::string& id) {
  auto view = registry.view<const Identifier>();
  for (auto entity: view) {
    bool has_id = HasComponent<Identifier>(entity);
    if (has_id && GetComponent<Identifier>(entity)->id == id) {
      return entity;
    }
  }
  return entt::null;
}

void Scene2::MoveEntity(entt::entity entity, entt::entity parent) {
  if (!registry.valid(entity)) {
    FF_LOG_WARN("MoveEntity: from entity invalid");
    return;
  }
  if (!registry.valid(parent)) {
    FF_LOG_WARN("MoveEntity: parent entity invalid");
    return;
  }
  Relationship* entity_r = GetComponent<Relationship>(entity);
  Relationship* entity_r_parent = GetComponent<Relationship>(entity_r->parent);
  Relationship* parent_r = GetComponent<Relationship>(parent);

  // 1. Remove 'entity' from its parent
  auto it = std::find(entity_r_parent->children.begin(), entity_r_parent->children.end(), entity);
  if (it != entity_r_parent->children.end()) {
    entity_r_parent->children.erase(it);
  }

  // 2. Set 'entity's new parent to 'parent'
  entity_r->parent = parent;

  // 3. Add 'entity' to its new 'parent'
  parent_r->children.push_back(entity);
}
}