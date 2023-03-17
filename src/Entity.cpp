#include "../include/Entity.hpp"
#include "../include/Scene.hpp"
#include "../include/Components.hpp"
#include "../include/Logger.hpp"
#include <iostream>
#include <algorithm>

namespace FF {

int Entity::entity_count = 0;

Entity* Entity::Deserialize(nlohmann::json j_entity, FF::Scene& scene, int index) {
  nlohmann::json j_comp_id            = j_entity["Identifier"];
  nlohmann::json j_comp_transform     = j_entity["Transform"];
  nlohmann::json j_comp_shape_render  = j_entity["ShapeRenderer"];
  nlohmann::json j_comp_sprite_render = j_entity["SpriteRenderer"];

  FF::Entity* e = nullptr;
  if (!j_comp_id.is_null()) {
    std::string id = j_comp_id["id"];
    e = scene.NewEntity(id);
  }
  else {
    return nullptr;
  }
  
  if (!j_comp_transform.is_null()) {
    glm::vec3 pos        = glm::vec3(j_comp_transform["position"][0], j_comp_transform["position"][1], j_comp_transform["position"][2]);
    glm::vec3 scl        = glm::vec3(j_comp_transform["scale"][0],    j_comp_transform["scale"][1],    j_comp_transform["scale"][2]);
    glm::vec3 rot        = glm::vec3(j_comp_transform["rotation"][0], j_comp_transform["rotation"][1], j_comp_transform["rotation"][2]);
    glm::vec3 rot_center = glm::vec3(j_comp_transform["rotation_center"][0], j_comp_transform["rotation_center"][1], j_comp_transform["rotation_center"][2]);
    e->AddComponent<Transform>();
    Transform* t = e->GetComponent<Transform>();
    t->position = pos;
    t->scale = scl;
    t->rotation = rot;
    t->rotation_center = rot_center;
  }

  if (!j_comp_shape_render.is_null()) {
    glm::vec4 col        = glm::vec4(j_comp_shape_render["color"][0], j_comp_shape_render["color"][1], j_comp_shape_render["color"][2], j_comp_shape_render["color"][3]);
    int shape            = j_comp_shape_render["shape"];
    e->AddComponent<ShapeRenderer>();
    ShapeRenderer* sr = e->GetComponent<ShapeRenderer>();
    sr->color = col;
    sr->shape = (ShapeRenderer::Shape) shape;
  }

  if (!j_comp_sprite_render.is_null()) {
    glm::vec4 color_tint = glm::vec4(j_comp_sprite_render["color_tint"][0], j_comp_sprite_render["color_tint"][1], j_comp_sprite_render["color_tint"][2], j_comp_sprite_render["color_tint"][3]);
    e->AddComponent<SpriteRenderer>();
    SpriteRenderer* sr = e->GetComponent<SpriteRenderer>();
    sr->color_tint = color_tint;
  }

  return e;
}

nlohmann::json Entity::Serialize(Entity* pEntity) {
  nlohmann::json j_entity;
  nlohmann::json j_comp_id;
  nlohmann::json j_comp_transform;
  nlohmann::json j_comp_shape_renderer;
  nlohmann::json j_comp_sprite_renderer;

  Identifier* id = pEntity->GetComponent<Identifier>();
  if (id) {
    j_comp_id["id"] = pEntity->GetComponent<Identifier>()->id;
    j_entity["Identifier"] = j_comp_id;
  }

  Transform* t = pEntity->GetComponent<Transform>();
  if (t) {
    j_comp_transform["position"] = { t->position.x, t->position.y, t->position.z };
    j_comp_transform["scale"] = { t->scale.x, t->scale.y, t->scale.z };
    j_comp_transform["rotation"] = { t->rotation.x, t->rotation.y, t->rotation.z };
    j_comp_transform["rotation_center"] = { t->rotation_center.x, t->rotation_center.y, t->rotation_center.z };
    j_entity["Transform"] = j_comp_transform;
  }

  ShapeRenderer* sr = pEntity->GetComponent<ShapeRenderer>();
  if (sr) {
    j_comp_shape_renderer["color"] = { sr->color.r, sr->color.g, sr->color.b, sr->color.a };
    j_comp_shape_renderer["shape"] = sr->shape;
    j_entity["ShapeRenderer"] = j_comp_shape_renderer;
  }
  
  SpriteRenderer* spr = pEntity->GetComponent<SpriteRenderer>();
  if (spr) {
    j_comp_sprite_renderer["color_tint"] = { spr->color_tint.r, spr->color_tint.r, spr->color_tint.r, spr->color_tint.r };
    j_entity["SpriteRenderer"] = j_comp_sprite_renderer;
  }
  return j_entity;
}

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
}