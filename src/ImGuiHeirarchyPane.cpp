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
  try {
    scene.NewEntity("Entity6", "Entity5");
  } catch (std::string& e) {
    std::cerr << e << std::endl;
  }
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
  if (root != nullptr && ShowNode(root)) {
    for (int i = 0; i < root->children.size(); i++) {
      if (root->children.at(i) != nullptr)
        ShowHeirarchy(root->children.at(i));
    }
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
