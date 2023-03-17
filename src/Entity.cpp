#include "../include/Entity.hpp"
#include "../include/Scene.hpp"
#include "../include/Components.hpp"
#include "../include/Logger.hpp"
#include <iostream>
#include <algorithm>

namespace FF {

int Entity::entity_count = 0;

// Deserialize the entity node and return a handle to the new entity
/*
Entity* Entity::Deserialize(YAML::Node entity_node, FF::Scene& scene, int index) {
  Entity* p_entity = nullptr;
  std::string id = "no-id";
  if (YAML::Node identifier = entity_node[0]["Identifier"]) {
    YAML::Node n_id = identifier["id"];
    id = n_id.as<std::string>();

    p_entity = scene.NewEntity(n_id.as<std::string>());
    Identifier* id = p_entity->GetComponent<Identifier>();
    // std::cout << *id << std::endl;

    FF_LOG_INFO("Added identifier comp");
  }
  else {
    FF_LOG_ERROR("Entity #{} has no identifier. New entity not allocated.", index);
    return nullptr;
  }

  // Transform component
  if (YAML::Node transform  = entity_node[1]["Transform"]) {
    YAML::Node n_pos  = transform[0]["position"];
    YAML::Node n_scl  = transform[1]["scale"];
    YAML::Node n_rot  = transform[2]["rotation"];
    YAML::Node n_roc = transform[3]["rotation_center"];
    glm::vec3 pos = glm::vec3(n_pos[0].as<float>(), n_pos[0].as<float>(), n_pos[0].as<float>());
    glm::vec3 scl = glm::vec3(n_scl[0].as<float>(), n_scl[0].as<float>(), n_scl[0].as<float>());
    glm::vec3 rot = glm::vec3(n_rot[0].as<float>(), n_rot[0].as<float>(), n_rot[0].as<float>());
    glm::vec3 roc = glm::vec3(n_roc[0].as<float>(), n_roc[0].as<float>(), n_roc[0].as<float>());
  
    p_entity->AddComponent<Transform>();
    Transform* t = p_entity->GetComponent<Transform>();
    t->position = pos;
    t->scale = scl;
    t->rotation = rot;
    t->rotation_center = roc;

    // std::cout << *t << std::endl;
  
    FF_LOG_INFO("Added transform comp");
  }

  // ShapeRenderer component
  if (YAML::Node shape_ren  = entity_node[2]["ShapeRenderer"]) {
    YAML::Node n_color = shape_ren[0]["color"];
    YAML::Node n_shape = shape_ren[1]["shape"];
    glm::vec4 color = glm::vec4(n_color[0].as<float>(), n_color[1].as<float>(), n_color[2].as<float>(), n_color[3].as<float>());
    int shape       = n_shape.as<int>();

    p_entity->AddComponent<ShapeRenderer>();
    ShapeRenderer* sr = p_entity->GetComponent<ShapeRenderer>();
    sr->color = color;
    sr->shape = ShapeRenderer::Shape(shape); //NOTE: is this how this works? Can you initialize an enum with an integer?

    // std::cout << *sr << std::endl;
    
    FF_LOG_INFO("Added shape renderer comp");
  }

  // SpriteRenderer component
  if (YAML::Node sprite_ren  = entity_node[3]["SpriteRenderer"]) {
    YAML::Node n_color_tint = sprite_ren[0]["color_tint"];
    glm::vec4 color_tint = glm::vec4(n_color_tint[0].as<float>(), n_color_tint[1].as<float>(), n_color_tint[2].as<float>(), n_color_tint[3].as<float>());

    p_entity->AddComponent<SpriteRenderer>();
    SpriteRenderer* sr = p_entity->GetComponent<SpriteRenderer>();
    sr->color_tint = color_tint;

    // std::cout << *sr << std::endl;
    
    FF_LOG_INFO("Added sprite renderer comp");
  }
  return p_entity;
}

YAML::Node Entity::Serialize(Entity* entity) {
  YAML::Emitter emitter;
  emitter << *entity;
  YAML::Node n = YAML::Load(emitter.c_str());
  return n;
}

//Serialize the entity using YAML::Emitter
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
*/

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