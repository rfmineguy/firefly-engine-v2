#include "../include/ImGuiHeirarchyPane.hpp"
#include "../include/Scene.hpp"

// TODO: Make the scene reference a smart pointer maybe
namespace FF {
ImGuiHeirarchyPane::ImGuiHeirarchyPane(FF::Scene& scene):
  ImGuiPane("Heirarchy"), scene(scene) {
  scene.NewEntity("Entity0");
  scene.NewEntity("Entity1");
}

ImGuiHeirarchyPane::~ImGuiHeirarchyPane() {}

void ImGuiHeirarchyPane::Show(FF::Window& window) {
  ImGui::Begin(name.c_str());
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
  
  ShowHeirarchy(scene.entity_tree);
  
  ImGui::End();
}

void ImGuiHeirarchyPane::ShowHeirarchy(FF::Scene::Node* root) {
  // traverse tree and render it
  if (ShowNode(root)) {
    // ..
    ImGui::TreePop();
  }
}

bool ImGuiHeirarchyPane::ShowNode(FF::Scene::Node* node) {
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
  bool opened = ImGui::TreeNodeEx(node->entity.GetComponent<Identifier>().id.c_str(), flags);
  // do other things
  return opened;
}
}
