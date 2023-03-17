#include "../include/Scene.hpp"
#include "../include/Logger.hpp"
#include <yaml-cpp-src/include/yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
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

void Scene::SerializeToFile(const std::string& filepath) {
  nlohmann::json scene;
  SerializeEntitiesRec(scene["Entities"], entity_tree_root);
  SerializeTreeRec(scene["Tree"], entity_tree_root);
  std::ofstream f(filepath, std::ios::out);
  f << scene.dump(4);
  f.close();
}

void Scene::SerializeEntitiesRec(nlohmann::json& entities_arr, FF::Entity* root) {
  if (root == nullptr) {
    return;
  }
  if (root != entity_tree_root) {
    entities_arr.push_back(FF::Entity::Serialize(root));
  }

  for (int i = 0; i < root->children.size(); i++) {
    SerializeEntitiesRec(entities_arr, root->children.at(i));
  }
}

void Scene::SerializeTreeRec(nlohmann::json& tree, Entity* root) {
  if (root == nullptr) {
    return;
  }
  tree["id"] = root->GetComponent<Identifier>()->id;
  for (int i = 0; i < root->children.size(); i++) {
    SerializeTreeRec(tree["children"][i], root->children.at(i));
  }
}

void Scene::DeserializeFromFile(const std::string& filepath) {
  std::ifstream f(filepath);
  if (!f.is_open()) {
    FF_LOG_ERROR("Failed to open {}", filepath);
    return;
  }
  nlohmann::json scene = nlohmann::json::parse(f);
  std::cout << "Parsed file" << std::endl;
  DeserializeEntities(scene["Entities"]);
  DeserializeTreeRec(scene["Tree"], entity_tree_root);

  f.close();
}

void Scene::DeserializeEntities(nlohmann::json entities) {
  if (entity_tree_root) {
    Clean(entity_tree_root);
  }
  
  for (int i = 0; i < entities.size(); i++) {
    FF::Entity* e = FF::Entity::Deserialize(entities[i], *this, i);
  }
}

void Scene::DeserializeTreeRec(nlohmann::json tree, FF::Entity* parent, int level) {
  std::string id = tree["id"];
  std::cout << level << std::endl;
  std::cout << id << std::endl;
  if (parent != entity_tree_root) {
    FF::Entity* e = FindEntityNodeRec(entity_tree_root, id);
    parent->AddChild(e);
    parent = e;
  }
  if (!tree.contains("children") || tree["children"].size() == 0 || level > 2)
    return;
  std::cout << tree["children"].dump(3) << std::endl;
  for (int i = 0; i < tree["children"].size(); i++) {
    DeserializeTreeRec(tree["children"][i], FindEntityNodeRec(parent, id), level + 1);
  }
}

/*
void Scene::DeserializeFromFile(const std::string& filepath) {
  // Clean out previous scene
  Clean(entity_tree_root);
  
  // https://jsonformatter.org/yaml-viewer
  try {
    const int ENTITIES_ARRAY = 0;
    const int TREE_ARRAY = 1;
    
    YAML::Node scene = YAML::LoadFile(filepath);
    std::cout << "Scene size : " << scene.size() << std::endl;
    
    //   END ENTITIES ARRAY PARSING
    YAML::Node entities_arr = scene[ENTITIES_ARRAY];

    std::cout << "Number of entities in scene file: " << entities_arr.size() << std::endl;
    std::cout << entities_arr << std::endl;
    
    for (int i = 0; i < entities_arr.size(); i++) {
      // YAML::Node n_entity_arr = entities_arr[i];
      YAML::Node n_entity = entities_arr[0];
      FF::Entity* p_entity = FF::Entity::Deserialize(n_entity, *this, i);
    }
    //   END ENTITIES ARRAY PARSING

    //   START TREE PARSING
    YAML::NodeType type;
    YAML::Node n_tree = scene[TREE_ARRAY]["Tree"];
    std::cout << n_tree << std::endl;
    std::cout << "Tree size" << n_tree.size() << std::endl;
    std::cout << "Tree type" << n_tree.Type() << std::endl;
    YAML::Node n_root = n_tree[0]["root"];
    std::cout << n_root.Type() << std::endl;
    DeserializeTreeFromFile(n_root, entity_tree_root);
   
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

  emitter << YAML::BeginSeq << YAML::Value;
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
      emitter << YAML::BeginSeq;
        SerializeTreeToFileRec(entity_tree_root, emitter);
      emitter << YAML::EndSeq;
    emitter << YAML::EndMap;

  emitter << YAML::EndSeq;
  
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

void Scene::DeserializeTreeFromFile(YAML::Node node, FF::Entity* parent) {
  FF_LOG_INFO("Deserialize node [type = {}, size = {}]", (int)node.Type(), node.size());
  if (node.size() == 0)
    return;
  for (int i = 0; i < node.size(); i++) {
    DeserializeTreeFromFile(node[i], nullptr);
  }
}
*/

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
