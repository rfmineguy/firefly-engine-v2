#include "../include/ImGuiInspectorPane.hpp"
#include "../include/Components.hpp"

namespace FF {
// NOTE: How am I going to notify this pane that a new entity has been selected
//       in a clean manor. DONT even think about making a singleton for this.
ImGuiInspectorPane::ImGuiInspectorPane(FF::Scene2& scene):
  ImGuiPane("InspectorPane"), scene(scene) {}

ImGuiInspectorPane::~ImGuiInspectorPane() {}

void ImGuiInspectorPane::Show(FF::Window& window) {
  if (!visible) {
    return;
  }
  ImGui::Begin(name.c_str());

  entt::entity selected_entity = scene.selection;
  bool is_selection_valid = scene.registry.valid(selected_entity);
  bool is_selection_dirty = scene.GetComponent<Relationship>(selected_entity)->is_dirty;

  if (is_selection_valid && !is_selection_dirty) {
    //
    // IDENTIFIER
    //
    if (scene.HasComponent<Identifier>(selected_entity)) {
      ImGui::Text("%s", scene.GetComponent<Identifier>(selected_entity)->id.c_str());
      ImGui::Separator();
    }

    //
    // TRANSFORM
    //
    if (scene.HasComponent<Transform>(selected_entity)) {
      static bool toggle_2d = true;
      ImGui::Checkbox("Toggle 2D", &toggle_2d);
      Transform* t = scene.GetComponent<Transform>(selected_entity);
      ImGui::Text("Transform");
      if (toggle_2d) {
        ImGui::DragFloat2("Position", &t->position.x, 0.05);
        ImGui::DragFloat2("Scale", &t->scale.x, 0.05f);
        ImGui::DragFloat2("Rotation", &t->rotation.x, 0.05f);               
      }
      else {
        ImGui::DragFloat3("Position", &t->position.x, 0.05);
        ImGui::DragFloat3("Scale", &t->scale.x, 0.05f);
        ImGui::DragFloat3("Rotation", &t->rotation.x, 0.05f);        
      }

      ImGui::Separator();
    }

    //
    // SHAPE RENDERER
    //
    if (scene.HasComponent<ShapeRenderer>(selected_entity)) {
      static int shape = 0;
      ShapeRenderer* s = scene.GetComponent<ShapeRenderer>(selected_entity);
      ImGui::Text("ShapeRenderer");
      ImGui::ColorEdit4("Color", &s->color.r);
      ImGui::Combo("Shape", &shape, "Rectangle\0Triangle\0Circle\0\0");
      ImGui::Separator();
    }

    //
    // SPRITE RENDERER
    //
    if (scene.HasComponent<SpriteRenderer>(selected_entity)) {
      SpriteRenderer* spr = scene.GetComponent<SpriteRenderer>(selected_entity);
      ImGui::Text("SpriteRenderer");
      ImGui::Separator();
    }
  }
  
  //
  // ADD COMPONENT POPUP
  //
  if (is_selection_valid && ImGui::Button("Add Component")) {
    ImGui::OpenPopup("AddComponentPopup");
  }
  if (ImGui::BeginPopup("AddComponentPopup")) {
    if (!scene.HasComponent<Transform>(selected_entity)) {
      if (ImGui::Button("Transform")) {
        scene.AddComponent<Transform>(selected_entity);
      }
    }
    if (!scene.HasComponent<ShapeRenderer>(selected_entity)) {
      if (ImGui::Button("ShapeRenderer")) {
        scene.AddComponent<ShapeRenderer>(selected_entity);
      }
    }
    if (!scene.HasComponent<SpriteRenderer>(selected_entity)) {
      if (ImGui::Button("SpriteRenderer")) {
        scene.AddComponent<SpriteRenderer>(selected_entity);
      }
    }

    ImGui::EndPopup();
  }
  ImGui::End();
}
}
