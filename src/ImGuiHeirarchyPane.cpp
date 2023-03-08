#include "../include/ImGuiHeirarchyPane.hpp"
#include "../include/Scene.hpp"

// TODO: Make the scene reference a smart pointer maybe
namespace FF {
ImGuiHeirarchyPane::ImGuiHeirarchyPane(FF::Scene& scene):
  ImGuiPane("Heirarchy"), scene(scene) {
  scene.NewEntity("Entity0");
  scene.NewEntity("Entity1");
  scene.NewEntity("Entity2");
  scene.NewEntity("Entity3");
  scene.NewEntity("Entity4");
  scene.NewEntity("Entity5");
  // scene.NewEntity("Entity6", "Entity5");
  scene.Traverse();
}

ImGuiHeirarchyPane::~ImGuiHeirarchyPane() {}

void ImGuiHeirarchyPane::Show(FF::Window& window) {
  if (!visible)
    return;
  ImGui::Begin(name.c_str());
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
  
  ShowHeirarchy(scene.entity_tree);
  
  ImGui::End();
}

void ImGuiHeirarchyPane::ShowHeirarchy(FF::Scene::Node* root) {
  // traverse tree and render it
  if (root == nullptr)
    return;
  if (ShowNode(root)) {
    for (int i = 0; i < root->children.size(); i++) {
      ShowHeirarchy(root->children.at(i));
    }
    ImGui::TreePop();
  }
}

bool ImGuiHeirarchyPane::ShowNode(FF::Scene::Node* node) {
  std::string node_id = node->entity.GetComponent<Identifier>().id;
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
  flags |= selection_id == node_id ? ImGuiTreeNodeFlags_Selected : 0; 

  bool opened = ImGui::TreeNodeEx(node->entity.GetComponent<Identifier>().id.c_str(), flags);
  if (ImGui::IsItemClicked(0) && node_id != "root") {
    selection_id = node_id;
  }
  // do other things
  return opened;
}
}
