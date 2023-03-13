#include "../include/Entity.hpp"
#include "../include/Components.hpp"
#include "../include/Logger.hpp"
#include <iostream>
#include <algorithm>

namespace FF {

int Entity::entity_count = 0;

Entity::Entity() {}

Entity::Entity(std::shared_ptr<entt::registry> reg)
:Entity("unnamed", reg) {}

Entity::Entity(const std::string& name, std::shared_ptr<entt::registry> reg)
:name(name), handle(reg->create()), registry_ptr(reg), parent(nullptr), is_dirty(false) {}

Entity::~Entity() {
  FF_LOG_WARN("Entity destructor {}", name);
  entity_count --;
  if (registry_ptr.lock()->valid(handle)) {
    registry_ptr.lock()->destroy(handle);
  }
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

// Serialize the entity
YAML::Emitter& operator<<(YAML::Emitter& out, const FF::Entity& e) {
  out << YAML::BeginMap << YAML::Key << "Entity" << YAML::Value;
  out << YAML::BeginSeq;
  if (e.HasComponent<Identifier>()) {
    out << YAML::BeginMap << YAML::Key << "Identifier" << YAML::Value;
      out << YAML::BeginMap << YAML::Key << "id" << YAML::Value << e.GetComponent<Identifier>()->id << YAML::EndMap;
    out << YAML::EndMap;
  }
  if (e.HasComponent<Transform>()) {
    Transform* t = e.GetComponent<Transform>();
    std::vector<float> pos = {t->position.x, t->position.y, t->position.z};
    std::vector<float> scale = {t->scale.x, t->scale.y, t->scale.z};
    std::vector<float> rotation = {t->rotation.x, t->rotation.y, t->rotation.z};
    std::vector<float> rotation_center = {t->rotation_center.x, t->rotation_center.y, t->rotation_center.z};

    out << YAML::BeginMap << YAML::Key << "Transform" << YAML::Value;
      out << YAML::BeginSeq;
      out << YAML::BeginMap << YAML::Key << "position"        << YAML::Value << YAML::Flow << pos             << YAML::EndMap;
      out << YAML::BeginMap << YAML::Key << "scale"           << YAML::Value << YAML::Flow << scale           << YAML::EndMap;
      out << YAML::BeginMap << YAML::Key << "rotation"        << YAML::Value << YAML::Flow << rotation        << YAML::EndMap;
      out << YAML::BeginMap << YAML::Key << "rotation_center" << YAML::Value << YAML::Flow << rotation_center << YAML::EndMap;
      out << YAML::EndSeq;
    out << YAML::EndMap;
  }
  if (e.HasComponent<ShapeRenderer>()) {
    ShapeRenderer* sr = e.GetComponent<ShapeRenderer>();
    std::vector<float> color = {sr->color.r, sr->color.g, sr->color.b, sr->color.a};
    
    out << YAML::BeginMap << YAML::Key << "ShapeRenderer" << YAML::Value;
      out << YAML::BeginSeq;
      out << YAML::BeginMap << YAML::Key << "color" << YAML::Value << YAML::Flow << color << YAML::EndMap;
      out << YAML::BeginMap << YAML::Key << "shape" << YAML::Value << sr->shape           << YAML::EndMap;
      out << YAML::EndSeq;
    out << YAML::EndMap;
  }
  if (e.HasComponent<SpriteRenderer>()) {
    SpriteRenderer* sr = e.GetComponent<SpriteRenderer>();
    std::vector<float> color_tint = {sr->color_tint.r, sr->color_tint.g, sr->color_tint.b, sr->color_tint.a};
    
    out << YAML::BeginMap << YAML::Key << "ShapeRenderer" << YAML::Value;
      out << YAML::BeginMap << YAML::Key << "color" << YAML::Value << YAML::Flow << color_tint << YAML::EndMap;
    out << YAML::EndMap;
  }
  out << YAML::EndSeq;
  out << YAML::EndMap;
  return out;
}
}