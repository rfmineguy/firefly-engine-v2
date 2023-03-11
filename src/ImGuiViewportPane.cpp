#include "../include/ImGuiViewportPane.hpp"
#include <glm-src/glm/gtx/transform.hpp>
#include <glm-src/glm/gtc/type_ptr.hpp>    // glm::value_ptr
#include <memory>

namespace FF {
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
  RenderScene();
  
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin(name.c_str());
  ImGui::Text("Resize viewport to see entities rendered");
  
  // Handle viewport resizing
  ImVec2 viewport_size = ImGui::GetContentRegionAvail();
  ImVec2 viewport_pos = ImGui::GetWindowPos();
  
  if (last_viewport_size.x != viewport_size.x || last_viewport_size.y != viewport_size.y) {
    fb->Resize(viewport_pos.x, viewport_pos.y, viewport_size.x, viewport_size.y);
    renderer.UpdateProjectionMatrix(viewport_pos.x, viewport_pos.y, viewport_size.x, viewport_size.y);
    last_viewport_size = viewport_size;
  }
  
  ImGui::Image((void*)(intptr_t)fb->GetColorAttachment(), viewport_size);
  ImGui::GetForegroundDrawList()->AddCircle(ImGui::GetMousePos(), 10, 0);

  ImGui::End();
  ImGui::PopStyleVar();
}

// ImGuiViewportPane is a friend of Scene
void ImGuiViewportPane::RenderScene() {
  RenderEntityNode(scene.entity_tree_root); //identity transformation
}

// Before cumulative transformation matrices just render the entities
void ImGuiViewportPane::RenderEntityNode(Entity* node, glm::mat4 transform) {
  if (node == nullptr)
    return;

  if (node->HasComponent<ShapeRenderer>() && node->HasComponent<Transform>()) {
    ShapeRenderer& rs = node->GetComponent<ShapeRenderer>();
    Transform& t = node->GetComponent<Transform>();
    transform = glm::translate(transform, t.position);
    transform = glm::scale(transform, t.scale);
    renderer.DrawQuad(transform, rs.color);
  }

  for (int i = 0; i < node->children.size(); i++) {
    RenderEntityNode(node->children.at(i), transform);
  }
}

void ImGuiViewportPane::DisplayMat4(glm::mat4 mat) {
}
}
