#include "../include/Entity.hpp"
#include <iostream>

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
:name(name), handle(reg->create()), registry_ptr(reg) {}

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