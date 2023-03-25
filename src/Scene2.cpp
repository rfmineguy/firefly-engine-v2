#include "../include/Scene2.hpp"
#include "../include/Components.hpp"
#include "../include/Util.hpp"
#include <utility>
#include <iostream>
#include <optional>

namespace FF {
int Scene2::entity_count = 0;

Scene2::Scene2()
:scene_root(registry.create()), selection(entt::null) {
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

nlohmann::json Scene2::Serialize() {
  nlohmann::json scene = {};
  registry.each([&](const auto entity) {
    scene.push_back(SerializeEntity(entity));
  });
  return scene;
}

void Scene2::Deserialize(nlohmann::json json) {
  for (int i = 0; i < json.size(); i++) {
    DeserializeEntity(json.at(i));
  }
}

nlohmann::json Scene2::SerializeEntity(entt::entity e) {
  nlohmann::json entity = {};
  nlohmann::json rel    = entity["Relationship"] = {};
  nlohmann::json id     = entity["Identifier"] = {};
  nlohmann::json trans  = entity["Transform"]  = {};
  nlohmann::json sh_ren = entity["ShapeRenderer"] = {};
  nlohmann::json sp_ren = entity["SpriteRenderer"] = {};
  // Relationship
  if (Relationship* rel_c = GetComponent<Relationship>(e)) {
    rel["parent"] = GetComponent<Identifier>(rel_c->parent)->id;
    nlohmann::json children = {};
    for (auto child : rel_c->children) {
      rel["children"].push_back(GetComponent<Identifier>(child)->id);
    }
    entity["Relationship"] = rel;
  }

  // Identifier
  if (Identifier* id_c = GetComponent<Identifier>(e)) {
    id["id"] = id_c->id;
    entity["Identifier"] = id;
  }

  // Transform
  if (Transform* t = GetComponent<Transform>(e)) {
    trans["position"]        = { t->position.x       , t->position.y       , t->position.z };
    trans["scale"]           = { t->scale.x          , t->scale.y          , t->scale.z    };
    trans["rotation"]        = { t->rotation.x       , t->rotation.y       , t->rotation.z };
    trans["rotation_center"] = { t->rotation_center.x, t->rotation_center.y, t->rotation_center.z };
    entity["Transform"] = trans;
  }

  if (ShapeRenderer* sr_c = GetComponent<ShapeRenderer>(e)) {
    sh_ren["color_tint"] = { sr_c->color.r, sr_c->color.g, sr_c->color.b, sr_c->color.a };
    sh_ren["shape"] = sr_c->shape;
    entity["ShapeRenderer"] = sh_ren;
  }

  if (SpriteRenderer* spr_c = GetComponent<SpriteRenderer>(e)) {
    sp_ren["color_tint"] = { spr_c->color_tint.r, spr_c->color_tint.g, spr_c->color_tint.b, spr_c->color_tint.a };
    entity["SpriteRenderer"] = sp_ren;
  }
  
  return entity;
}

entt::entity Scene2::DeserializeEntity(nlohmann::json json) {
  nlohmann::json rel    = json["Relationship"];
  nlohmann::json id     = json["Identifier"];
  nlohmann::json trans  = json["Transform"];
  nlohmann::json sh_ren = json["ShapeRenderer"];
  nlohmann::json sp_ren = json["SpriteRenderer"];

  std::cout << "===============" << std::endl;
  if (!rel.is_null()) {
    std::string parent = rel["parent"];
    std::vector<std::string> children_vec;
    
    nlohmann::json children = rel["children"];
    for (int i = 0; i < children.size(); i++) {
      children_vec.push_back(children.at(i));
    }


    std::cout << "Relationship: " << std::endl;
    std::cout << "\tparent: " << parent << std::endl;
    std::cout << "\tchildren: [";
    for (int i = 0; i < children_vec.size(); i++) {
      std::cout << children_vec.at(i);
      if (i != children_vec.size() - 1)
        std::cout << ", ";
    }
    std::cout << "]" << std::endl;
  }

  if (!id.is_null()) {
    std::string id_s = id["id"];

    std::cout << "Identifier: " << std::endl;
    std::cout << "\tid: " << id_s << std::endl;
  }

  if (!trans.is_null()) {
    glm::vec3 position        = glm::vec3(trans["position"][0], trans["position"][1], trans["position"][2]);
    glm::vec3 scale           = glm::vec3(trans["scale"][0], trans["scale"][1], trans["scale"][2]);
    glm::vec3 rotation        = glm::vec3(trans["rotation"][0], trans["rotation"][1], trans["rotation"][2]);
    glm::vec3 rotation_center = glm::vec3(trans["rotation_center"][0], trans["rotation_center"][1], trans["rotation_center"][2]);

    std::cout << "Transform: " << std::endl;
    std::cout << "\t"; FF::Util::DisplayVec3(position);
    std::cout << "\t"; FF::Util::DisplayVec3(scale);
    std::cout << "\t"; FF::Util::DisplayVec3(rotation);
    std::cout << "\t"; FF::Util::DisplayVec3(rotation_center);
  }

  if (!sh_ren.is_null()) {
    glm::vec3 color_tint = glm::vec3(sh_ren["color_tint"][0], sh_ren["color_tint"][1], sh_ren["color_tint"][2]);
    int shape = sh_ren["shape"];

    std::cout << "Shape Renderer: " << std::endl;
    std::cout << "\t"; FF::Util::DisplayVec3(color_tint);
    std::cout << "\tshape: " << shape << std::endl;
  }

  if (!sp_ren.is_null()) {
    glm::vec3 color_tint = glm::vec3(sp_ren["color_tint"][0], sp_ren["color_tint"][1], sp_ren["color_tint"][2]);

    std::cout << "Sprite Renderer: " << std::endl;
    std::cout << "\t"; FF::Util::DisplayVec3(color_tint);
  }
  std::cout << "===============" << std::endl;
}
}