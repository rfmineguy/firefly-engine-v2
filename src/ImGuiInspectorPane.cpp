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
      ImGui::Separator();
    }
    if (scene.selected_entity->HasComponent<Transform>()) {
      static bool toggle_2d = true;
      ImGui::Checkbox("Toggle 2D", &toggle_2d);
      Transform& t = scene.selected_entity->GetComponent<Transform>();
      ImGui::Text("Transform");
      if (toggle_2d) {
        ImGui::DragFloat2("Position", &t.position.x, 0.05);
        ImGui::DragFloat2("Scale", &t.scale.x, 0.05f);
        ImGui::DragFloat2("Rotation", &t.rotation.x, 0.05f);               
      }
      else {
        ImGui::DragFloat3("Position", &t.position.x, 0.05);
        ImGui::DragFloat3("Scale", &t.scale.x, 0.05f);
        ImGui::DragFloat3("Rotation", &t.rotation.x, 0.05f);        
      }

      ImGui::Separator();
    }
    if (scene.selected_entity->HasComponent<ShapeRenderer>()) {
      ShapeRenderer& s = scene.selected_entity->GetComponent<ShapeRenderer>();
      ImGui::Text("ShapeRenderer");
      ImGui::ColorEdit4("Color", &s.color.r);
      ImGui::Separator();
    }
    if (scene.selected_entity->HasComponent<SpriteRenderer>()) {
      SpriteRenderer& spr = scene.selected_entity->GetComponent<SpriteRenderer>();
      ImGui::Text("SpriteRenderer");
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
    if (!scene.selected_entity->HasComponent<SpriteRenderer>()) {
      if (ImGui::Button("SpriteRenderer")) {
        scene.selected_entity->AddComponent<SpriteRenderer>();
      }
    }

    ImGui::EndPopup();
  }
  ImGui::End();
}
}
