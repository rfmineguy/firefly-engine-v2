#include "../include/Entity.hpp"

namespace FF {

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

Entity::~Entity() {}

bool Entity::operator==(const Entity& e) const {
  return handle == e.handle;
}

bool Entity::operator!=(const Entity& e) const {
  return !(operator==(e));
}

template <typename T>
T& Entity::GetComponent() {
  return T();
}

template <typename T>
void Entity::AddComponent() {
  
}

template <typename T>
bool Entity::HasComponent() {
  return false;
}

const std::string& Entity::GetName() const {
  return name;
}
}