#include "../include/Entity.hpp"
#include "../include/Components.hpp"
#include <iostream>
#include <algorithm>

namespace FF {

int Entity::entity_count = 0;
Entity Entity::InvalidEntity() {
  Entity e;
  e.name = "Invalid";
  return e;
}

Entity::Entity(std::shared_ptr<entt::registry> reg)
:Entity("unnamed", reg) {}

Entity::Entity(const std::string& name, std::shared_ptr<entt::registry> reg)
:name(name), handle(reg->create()), registry_ptr(reg), parent(nullptr) {}

Entity::Entity() {}

Entity::~Entity() {
  entity_count --;
}

bool Entity::operator==(const Entity& e) const {
  return handle == e.handle;
}

bool Entity::operator!=(const Entity& e) const {
  return !(operator==(e));
}

const std::string& Entity::GetName() const {
  return name;
}

Entity* Entity::AddChild(Entity* e) {
  // remove e from its parent
  if (e->parent) {
    auto it = std::find(e->parent->children.begin(), e->parent->children.end(), e);
    if (it != e->parent->children.end()) {
      e->parent->children.erase(it);
    }
  }

  e->parent = this;
  children.push_back(e);
  
  return e;
}

/*
  GetComponent<T>
  AddComponent<T>
  and HashComponent<T>

  defined in the header file
*/
}