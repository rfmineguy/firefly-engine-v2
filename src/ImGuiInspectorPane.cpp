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
    ImGui::Text("%s", scene.selected_entity->GetComponent<Identifier>().id.c_str());
  }
  ImGui::End();
}
}
