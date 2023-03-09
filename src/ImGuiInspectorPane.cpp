#include "../include/ImGuiInspectorPane.hpp"

namespace FF {
// NOTE: How am I going to notify this pane that a new entity has been selected
//       in a clean manor. DONT even think about making a singleton for this.
ImGuiInspectorPane::ImGuiInspectorPane(FF::Scene& scene):
  ImGuiPane("InspectorPane"), scene(scene) {}

ImGuiInspectorPane::~ImGuiInspectorPane() {}

void ImGuiInspectorPane::Show(FF::Window& window) {
  if (!visible) {
    return;
  }
  ImGui::Begin(name.c_str());
  if (scene.selected_entity) {
    if (scene.selected_entity->HasComponent<Identifier>()) {
      ImGui::Text("%s", scene.selected_entity->GetComponent<Identifier>().id.c_str());
    }
    if (scene.selected_entity->HasComponent<Transform>()) {
      Transform& t = scene.selected_entity->GetComponent<Transform>();
      ImGui::Text("Transform");
      ImGui::DragFloat3("Position", &t.position.x);
      ImGui::DragFloat3("Scale", &t.scale.x);
      ImGui::DragFloat3("Rotation", &t.rotation.x);
      ImGui::Separator();
    }
    if (scene.selected_entity->HasComponent<ShapeRenderer>()) {
      ShapeRenderer& s = scene.selected_entity->GetComponent<ShapeRenderer>();
      ImGui::Text("ShapeRenderer");
      ImGui::DragFloat4("Color", &s.color.r, 1, 0, 255);
      ImGui::Separator();
    }
  }
  
  if (scene.selected_entity && ImGui::Button("Add Component")) {
    ImGui::OpenPopup("AddComponentPopup");
  }
  if (ImGui::BeginPopup("AddComponentPopup")) {
    if (!scene.selected_entity->HasComponent<Transform>()) {
      if (ImGui::Button("Transform")) {
        scene.selected_entity->AddComponent<Transform>();
      }
    }
    if (!scene.selected_entity->HasComponent<ShapeRenderer>()) {
      if (ImGui::Button("ShapeRenderer")) {
        scene.selected_entity->AddComponent<ShapeRenderer>();
      }
    }
    ImGui::EndPopup();
  }
  ImGui::End();
}
}
