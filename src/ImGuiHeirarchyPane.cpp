#include "../include/ImGuiHeirarchyPane.hpp"

ImGuiHeirarchyPane::ImGuiHeirarchyPane(): ImGuiPane("Heirarchy") {}

ImGuiHeirarchyPane::~ImGuiHeirarchyPane() {}

void ImGuiHeirarchyPane::Show(FF::Window& window) {
  ImGui::Begin(name.c_str());
  
  ImGui::End();
}

void ImGuiHeirarchyPane::ShowSceneNode(FF::Scene::Node* scene_node) {
  
}