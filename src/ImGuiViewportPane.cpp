#include "../include/ImGuiViewportPane.hpp"

namespace FF {
ImGuiViewportPane::ImGuiViewportPane(FF::Scene& _scene):
  ImGuiPane("Viewport"), scene(_scene) {}

ImGuiViewportPane::~ImGuiViewportPane() {}

void ImGuiViewportPane::Show(FF::Window& window) {
  if (!visible)
    return;

  std::shared_ptr<FF::Framebuffer> fb = window.GetFramebuffer();
  renderer.SetTargetFramebuffer(fb);
  renderer.ClearColor(100, 100, 100);
  renderer.DrawQuad();
  // RenderScene();
  
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin(name.c_str());
  
  // Handle viewport resizing
  ImVec2 viewport_size = ImGui::GetContentRegionAvail();
  if (last_viewport_size.x != viewport_size.x || last_viewport_size.y != viewport_size.y) {
    fb->Resize(viewport_size.x, viewport_size.y);
    last_viewport_size = viewport_size;
  }
  
  ImGui::Image((void*)(intptr_t)fb->GetColorAttachment(), viewport_size);
  ImGui::End();
  ImGui::PopStyleVar();
}

// ImGuiViewportPane is a friend of Scene
void ImGuiViewportPane::RenderScene() {
  // Use the renderer to render the scene objects
  RenderEntityNode(scene.entity_tree_root);
}

// Before cumulative transformation matrices just render the entities
void ImGuiViewportPane::RenderEntityNode(Entity* node) {
  if (node == nullptr)
    return;
  // render the entity
}
}
