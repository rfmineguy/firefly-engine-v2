#include "../include/ImGuiViewportPane.hpp"

ImGuiViewportPane::ImGuiViewportPane(FF::Scene& _scene):
ImGuiPane("Viewport"), scene(_scene) {
}

ImGuiViewportPane::~ImGuiViewportPane() {}

void ImGuiViewportPane::Show(FF::Window& window) {
  if (!visible)
    return;

  std::shared_ptr<FF::Framebuffer> fb = window.GetFramebuffer();
  renderer.SetTargetFramebuffer(fb);
  renderer.ClearColor(100, 100, 100);
  
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

}