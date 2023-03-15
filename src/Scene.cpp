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
  // Clean out previous scene
  Clean(entity_tree_root);
  
  // https://jsonformatter.org/yaml-viewer
  try {
    YAML::Node scene = YAML::LoadFile(filepath);
    std::cout << "Scene size : " << scene.size() << std::endl;
    
    //   END ENTITIES ARRAY PARSING
    YAML::Node entities_arr = scene["Scene"]["Entities"];
    std::cout << "Number of entities in scene file: " << entities_arr.size() << std::endl;
    std::cout << entities_arr << std::endl;
    
    for (int i = 0; i < entities_arr.size(); i++) {
      YAML::Node n_entity_arr = entities_arr[i];
      YAML::Node n_entity = n_entity_arr["Entity"];
      FF::Entity* p_entity = FF::Entity::Deserialize(n_entity, *this, i);
    }
    //   END ENTITIES ARRAY PARSING

    //   START TREE PARSING
    YAML::Node n_tree = scene["Scene"]["Tree"];
    std::cout << "TREEEE" << std::endl;
    
    //   END TREE PARSING
    
  } catch (YAML::ParserException e) {
    FF_LOG_ERROR("{}", e.what());
  }
  
  Traverse();
  FF_LOG_INFO("Deserialized scene");
}

void Scene::SerializeToFile(const std::string& filepath) {
  FF_LOG_INFO("Serializing scene");
  YAML::Emitter emitter;

  //emitter << YAML::BeginSeq << YAML::Key << "Scene" << YAML::Value;
    // =========================================
    //   Serialize entities using yaml-cpp
    // =========================================
    emitter << YAML::BeginMap << YAML::Key << "Entities" << YAML::Value;
      emitter << YAML::BeginSeq;
        SerializeEntitiesToFileRec(entity_tree_root, emitter);
      emitter << YAML::EndSeq;
    emitter << YAML::EndMap;

    // =========================================
    //   Serialize tree structure using yaml-cpp
    // =========================================
    emitter << YAML::BeginMap << YAML::Key << "Tree" << YAML::Value;
      SerializeTreeToFileRec(entity_tree_root, emitter);
    emitter << YAML::EndMap;
  //emitter << YAML::EndSeq;
  
  // =========================================
  //   Write emitter to file
  // =========================================
  std::ofstream of(filepath);
  if (!of.is_open()) {
    FF_LOG_CRIT("Failed to open {}", filepath);
    return;
  }
  of << emitter.c_str();
  of.close();
  
  FF_LOG_INFO("Serialized scene");
  std::cout << emitter.c_str() << std::endl;
}

void Scene::SerializeEntitiesToFileRec(Entity* root, YAML::Emitter& emitter) {
  if (root == nullptr) {
    return;
  }
  if (root != entity_tree_root) {
    emitter << Entity::Serialize(root);
  }
  for (int i = 0; i < root->children.size(); i++) {
    SerializeEntitiesToFileRec(root->children.at(i), emitter);
  }
}

void Scene::SerializeTreeToFileRec(Entity* root, YAML::Emitter& emitter) {
  if (root == nullptr) {
    return;
  }
  emitter << YAML::BeginMap << YAML::Key;
    emitter << root->GetComponent<Identifier>()->id << YAML::Value;
    emitter << YAML::BeginSeq;
    for (int i = 0; i < root->children.size(); i++) {
      SerializeTreeToFileRec(root->children.at(i), emitter);
    }
    emitter << YAML::EndSeq;
  emitter << YAML::EndMap;
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
