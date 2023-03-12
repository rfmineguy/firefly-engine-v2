#include "../include/Entity.hpp"
#include "../include/Components.hpp"
#include "../include/Logger.hpp"
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
:name(name), handle(reg->create()), registry_ptr(reg), parent(nullptr), is_dirty(false) {}

Entity::Entity() {}

Entity::~Entity() {
  FF_LOG_WARN("Entity destructor {}", name);
  entity_count --;
  registry_ptr.lock()->destroy(handle);
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

Entity* Entity::RemoveChild(Entity* e) {
  auto it = std::find(children.begin(), children.end(), e);
  if (it != children.end()) {
    children.erase(it);
  }
  return e;
}

void Entity::MarkDirty(bool dirty) {
  MarkDirtyRec(this, dirty);
}

void Entity::MarkDirtyRec(Entity* e, bool dirty) {
  if (e == nullptr) {
    return;
  }
  e->is_dirty = true;
  for (int i = 0; i < e->children.size(); i++) {
    MarkDirtyRec(e->children.at(i), dirty);
  }
}

/*
  GetComponent<T>
  AddComponent<T>
  and HashComponent<T>

  defined in the header file
*/
}