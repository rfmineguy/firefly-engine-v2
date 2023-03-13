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
  // https://jsonformatter.org/yaml-viewer
  try {
    YAML::Node scene = YAML::LoadFile(filepath);
    YAML::Node entities_arr = scene["Entities"];
    std::cout << "Number of entities in scene file: " << entities_arr.size() << std::endl;
    std::cout << entities_arr << std::endl;
    if (entities_arr.IsSequence()) {
      std::cout << "IsSequence" << std::endl;
    }
    
    for (int i = 0; i < entities_arr.size(); i++) {
      std::cout << "======" << std::endl;
      std::cout << "Entity" << std::endl;
      std::cout << "======" << std::endl;
      YAML::Node entity = entities_arr[i];
      YAML::Node entity_ = entity["Entity"];
      if (YAML::Node identifier = entity_[0]["Identifier"]) {
        YAML::Node id = identifier["id"];
        std::cout << "Identifier:" << std::endl;
        std::cout << "\t" << id.as<std::string>() << std::endl;
      }
      if (YAML::Node transform  = entity_[1]["Transform"]) {
        YAML::Node n_pos  = transform[0]["position"];
        YAML::Node n_scl  = transform[1]["scale"];
        YAML::Node n_rot  = transform[2]["rotation"];
        YAML::Node n_roc = transform[3]["rotation_center"];
        glm::vec3 pos = glm::vec3(n_pos[0].as<float>(), n_pos[0].as<float>(), n_pos[0].as<float>());
        glm::vec3 scl = glm::vec3(n_scl[0].as<float>(), n_scl[0].as<float>(), n_scl[0].as<float>());
        glm::vec3 rot = glm::vec3(n_rot[0].as<float>(), n_rot[0].as<float>(), n_rot[0].as<float>());
        glm::vec3 roc = glm::vec3(n_roc[0].as<float>(), n_roc[0].as<float>(), n_roc[0].as<float>());
        
        std::cout << "Transform:" << std::endl;
        std::cout << "\tpos   => x: " << pos[0] << ", y: " << pos[1] << ", z: " << pos[2]<< std::endl;
        std::cout << "\tscl   => x: " << scl[0] << ", y: " << scl[1] << ", z: " << scl[2]<< std::endl;
        std::cout << "\trot   => x: " << rot[0] << ", y: " << rot[1] << ", z: " << rot[2]<< std::endl;
        std::cout << "\troc   => x: " << roc[0] << ", y: " << roc[1] << ", z: " << roc[2]<< std::endl;
      }
      if (YAML::Node shape_ren  = entity_[2]["ShapeRenderer"]) {
        YAML::Node n_color = shape_ren[0]["color"];
        YAML::Node n_shape = shape_ren[1]["shape"];
        glm::vec3 color = glm::vec3(n_color[0].as<float>(), n_color[0].as<float>(), n_color[0].as<float>());
        int shape       = n_shape.as<int>();

        std::cout << "ShapeRenderer:" << std::endl;
        std::cout << "\tcolor   => x: " << color[0] << ", y: " << color[1] << ", z: " << color[2]<< std::endl;
        std::cout << "\tshape   => " << shape << std::endl;
      }
    }
  } catch (YAML::ParserException e) {
    FF_LOG_ERROR("{}", e.what());    
  }
  
  Traverse();
  FF_LOG_INFO("Deserialized scene");
}

void Scene::SerializeToFile(const std::string& filepath) {
  FF_LOG_INFO("Serializing scene");
  YAML::Emitter emitter;
  
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
