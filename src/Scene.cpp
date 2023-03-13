#include "../include/Scene.hpp"
#include "../include/Logger.hpp"
#include <yaml-cpp-src/include/yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>
#include <memory>

namespace FF {

Scene::Scene(): entity_count(0) {
  registry = std::make_shared<entt::registry>();
  entity_tree_root = new Entity("root", registry);
  entity_tree_root->AddComponent<Identifier>("root");
  entity_tree_root->AddComponent<Transform>();
  entity_tree_root->AddComponent<ShapeRenderer>();
  FF_LOG_INFO("Created scene tree");
}

Scene::~Scene() {
  Clean(entity_tree_root);
  delete entity_tree_root;
  entity_tree_root = nullptr;
  FF_LOG_INFO("Deleted scene tree");
}

void Scene::Clean(Entity* root) {
  if (root == nullptr) {
    return;
  }
  for (int i = 0; i < root->children.size(); i++) {
    Clean(root->children[i]);
    delete root->children[i];
    root->children[i] = nullptr;
  }
}

Entity* Scene::NewEntity(const std::string& name) {
  Entity *e = new Entity(name, registry);
  e->AddComponent<Identifier>(name);
  e->AddComponent<Transform>();
  e->AddComponent<ShapeRenderer>();
  entity_tree_root->AddChild(e);
  entity_count++;
  return e;
}

void Scene::DeleteEntity(Entity*& entity) {
  Clean(entity);
  Entity* p = entity->parent;
  p->RemoveChild(entity);
  delete entity;
  entity = nullptr;
}

Entity Scene::FindEntity(const std::string& id) {
  return FindEntityRec(entity_tree_root, id);
}

void Scene::Traverse() {
  FF_LOG_DEBUG("Traversing scene");
  TraverseRec(entity_tree_root, 0);
}

void Scene::TraverseRec(Entity* root, int level) {
  if (root == nullptr)
    return;
  for (int i = 0; i < level; i++) {
    std::cout << "  ";
  }
  std::cout << "\\_";
  std::cout << root->GetComponent<Identifier>()->id << std::endl;
  for (int i = 0; i < root->children.size(); i++) {
    TraverseRec(root->children[i], level+1);
  }
}

Entity* Scene::FindEntityNode(const std::string& id) {
  return FindEntityNodeRec(entity_tree_root, id);
}

int Scene::GetEntityCount() const {
  return entity_count;
}

void Scene::DeserializeFromFile(const std::string& filepath) {
  try {
    YAML::Node scene = YAML::LoadFile(filepath);
    // std::cout << scene << std::endl;
    YAML::Node entities_arr = scene["Entities"];
    std::cout << entities_arr << std::endl;
  } catch (YAML::ParserException e) {
    FF_LOG_ERROR("{}", e.what());    
  }
  
  // this is not processed
  /*
  try {
    FF_LOG_INFO("Deserializing {}", filepath);
    YAML::Node scene = YAML::LoadFile(filepath);
    FF_LOG_INFO("Scene Node => ");
    std:: cout << scene;
    YAML::Node entities = scene["Entities"];
    std::cout << entities << std::endl;
    
    for (int i = 0; i < entities.size(); i++) {
      std::cout << "==========================" << std::endl;
      // Parse identifier component
      if (YAML::Node id = entities[i]["IdentifierComponent"]) {
        std::string id_str = id["id"].as<std::string>();
        std::cout << "Identifier" << std::endl;
        std::cout << "\tId   =>   id: " << id_str << std::endl;
      }

      //   Parse transform component
      if (YAML::Node trans = entities[i]["TransformComponent"]) {
        YAML::Node position_node = trans["position"];
        YAML::Node rotation_node = trans["rotation"];
        YAML::Node scale_node = trans["scale"];
        glm::vec3 pos = glm::vec3(
          position_node[0].as<float>(),
          position_node[1].as<float>(),
          position_node[2].as<float>()
        );
        glm::vec3 scale = glm::vec3(
          scale_node[0].as<float>(),
          scale_node[1].as<float>(),
          scale_node[2].as<float>()
        );
        glm::vec3 rotation = glm::vec3(
          rotation_node[0].as<float>(),
          rotation_node[1].as<float>(),
          rotation_node[2].as<float>()
        );
        std::cout << "Transform" << std::endl;
        std::cout << "\tPos   =>   x: " << pos.x      << ", y: " << pos.y      << ", z: " << pos.z << std::endl;
        std::cout << "\tScale =>   x: " << scale.x    << ", y: " << scale.y    << ", z: " << scale.z << std::endl;
        std::cout << "\tRot   =>   x: " << rotation.x << ", y: " << rotation.y << ", z: " << rotation.z << std::endl;
      }
      
      //  Parse shape renderer component
      if (YAML::Node shape_renderer = entities[i]["ShapeRendererComponent"]) {
        YAML::Node color_node = shape_renderer["color"];
        YAML::Node shape_node = shape_renderer["shape"];
        glm::vec4 color = glm::vec4(
          color_node[0].as<float>(),
          color_node[1].as<float>(),
          color_node[2].as<float>(),
          color_node[3].as<float>()
        );
        std::string shape_str = shape_node.as<std::string>();
        
        std::cout << "ShapeRenderer" << std::endl;
        std::cout << "\tColor =>  r: " << color.r << ", g: " << color.g << ", b: " << color.b << ", a: " << color.a << std::endl;
        std::cout << "\tShape =>  shape: " << shape_str << std::endl;
      }
      if (YAML::Node sprite_renderer = entities[i]["SpriteRendererComponent"]) {
        YAML::Node color_tint_node = sprite_renderer["color_tint"];
        glm::vec4 color = glm::vec4(
          color_tint_node[0].as<float>(),
          color_tint_node[1].as<float>(),
          color_tint_node[2].as<float>(),
          color_tint_node[3].as<float>()
        );
        
        std::cout << "SpriteRenderer" << std::endl;
        std::cout << "\tColor =>  r: " << color.r << ", g: " << color.g << ", b: " << color.b << ", a: " << color.a << std::endl;
      }
    }
  } catch (YAML::ParserException e) {
    FF_LOG_ERROR("{}", e.what());
  }
  */

  FF_LOG_INFO("Deserialized scene");
}

void Scene::SerializeToFile(const std::string& filepath) {
  FF_LOG_INFO("Serializing scene");
  YAML::Emitter emitter;
  emitter.SetIndent(4);
  
  // Use emitter
  emitter << YAML::BeginMap << YAML::Key << "Entities";
  emitter << YAML::BeginSeq;
    SerializeToFileRec(entity_tree_root, emitter);
  emitter << YAML::EndSeq;
  emitter << YAML::EndMap;
  
  // Write emitter to file
  std::ofstream of(filepath);
  if (!of.is_open()) {
    FF_LOG_CRIT("Failed to open {}", filepath);
    return;
  }
  // std::cout << emitter.c_str() << std::endl;
  if (emitter.good()) {
    FF_LOG_INFO("Good format");
  }
  else {
    FF_LOG_INFO("Bad format");
  }
  of << emitter.c_str();
  of.close();
  
  FF_LOG_INFO("Serialized scene");
  std::cout << emitter.c_str() << std::endl;
}

void Scene::SerializeToFileRec(Entity* root, YAML::Emitter& emitter) {
  if (root == nullptr) {
    return;
  }
  if (root != entity_tree_root) {
    emitter << *root;
  }
  for (int i = 0; i < root->children.size(); i++) {
    SerializeToFileRec(root->children.at(i), emitter);
  }
}


// Use depth or breadth first search
//  Probably depth first search
//  https://www.youtube.com/watch?v=TtAflDtqwVg
//  https://en.wikipedia.org/wiki/Depth-first_search
Entity* Scene::FindEntityNodeRec(Entity* root, const std::string& id) {
  Entity* found = nullptr;
  if (root == nullptr)
    return nullptr;
  Identifier* id_comp = root->GetComponent<Identifier>();
  if (id_comp && id_comp->id == id)
    return root;

  if (root->children.size() == 0)
    return nullptr;

  for (int i = 0; i < root->children.size(); i++) {
    found = FindEntityNodeRec(root->children.at(i), id);
    if (found != nullptr) {
      break;
    }
  }
  return found;
}

Entity Scene::FindEntityRec(Entity* root, const std::string& id) {
  Entity* found = FindEntityNodeRec(root, id);
  if (found == nullptr)
    throw std::string("No entity found");
  return *found;
}

void Scene::InsertEntity(Entity* root, Entity& e) {
  InsertEntity(entity_tree_root, e, *root);
}

void Scene::InsertEntity(Entity* root, Entity& e, Entity parent) {
  if (root == nullptr)
    return;
  if (*root == parent) {
    root->children.push_back(new Entity(e));
    return;
  }
  for (int i = 0; i < root->children.size(); i++) {
    InsertEntity(root->children[i], e, parent);
  }
}
}
