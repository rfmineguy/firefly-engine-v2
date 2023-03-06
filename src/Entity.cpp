#include "../include/Entity.hpp"
#include <iostream>

namespace FF {

int Entity::entity_count = 0;
Entity Entity::InvalidEntity() {
  Entity e;
  e.name = "Invalid";
  return e;
}

Entity::Entity(const std::string& name, entt::entity entt_handle)
:name(name), handle(entt_handle) {}

Entity::Entity(entt::entity entt_handle)
:name("unnamed"), handle(entt_handle) {}

Entity::Entity(): is_valid(false) {}

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

/*
  GetComponent<T>
  AddComponent<T>
  and HashComponent<T>

  defined in the header file
*/
}