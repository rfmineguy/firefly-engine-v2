#include "../include/ImGuiViewportPane.hpp"
#include "../include/Logger.hpp"
#include <glm-src/glm/gtx/transform.hpp>
#include <glm-src/glm/gtc/type_ptr.hpp>    // glm::value_ptr
#include <memory>

namespace FF {
ImGuiViewportPane::ImGuiViewportPane(FF::Scene2& _scene):
  ImGuiPane("Viewport"), scene(_scene), move_icon("res/textures/icons/icon_move.png"), eyeball_icon("res/textures/icons/icon_eyeball.png") {
}

ImGuiViewportPane::~ImGuiViewportPane() {}

void ImGuiViewportPane::Show(FF::Window& window) {
  if (!visible)
    return;

  camera.Update();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin(name.c_str());
  ImGui::Text("Resize viewport to see entities rendered");
  
  // ========================================
  // Render the scene to the framebuffer
  // ========================================
  std::shared_ptr<FF::Framebuffer> fb = window.GetFramebuffer();
  renderer.ClearColor(0.6f, 0.6f, 0.6f, 1.f, fb);
  RenderScene(fb);

  // ========================================
  // Handle what happens when the viewport gets resized
  // ========================================
  ImVec2 viewport_size = ImGui::GetContentRegionAvail();
  ImVec2 viewport_pos = ImGui::GetWindowPos();
  static bool first = true;
  if (first) {
    fb->Resize(viewport_pos.x, viewport_pos.y, viewport_size.x, viewport_size.y);
    camera.SetProjSize(viewport_size.x, viewport_size.y);
    glViewport(0, 0, viewport_size.x, viewport_size.y);
    first = false;
  }
  if (last_viewport_size.x != viewport_size.x || last_viewport_size.y != viewport_size.y) {
    fb->Resize(viewport_pos.x, viewport_pos.y, viewport_size.x, viewport_size.y);
    camera.SetProjSize(viewport_size.x, viewport_size.y);
    last_viewport_size = viewport_size;
    glViewport(0, 0, viewport_size.x, viewport_size.y);
  }
  
  // ========================================
  // Render the framebuffer to the viewport content region
  // ========================================
  ImGui::Image((void*)(intptr_t)fb->GetColorAttachment(), viewport_size);

  // ========================================
  // Handle right click to pan
  // ========================================
  if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
    ImVec2 m_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    FF_LOG_INFO("Mouse dragging. Mouse delta dx: {} dy: {}", m_delta.x, m_delta.y);
    camera.DeltaPos(m_delta.x, -m_delta.y);
    
    ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
    ImVec2 mouse_pos = ImGui::GetMousePos();
    ImVec2 icon_size = { (float) move_icon.GetWidth() / 8, (float) move_icon.GetHeight() / 8 };
    ImVec2 icon_center = { mouse_pos.x - icon_size.x / 2, mouse_pos.y - icon_size.y / 2 };
    
    ImGui::GetForegroundDrawList()->AddImage((void*)(intptr_t) eyeball_icon.Handle(), icon_center, ImVec2{mouse_pos.x + icon_size.x, mouse_pos.y + icon_size.y});
  }

  ImGui::End();
  ImGui::PopStyleVar();
}

// ImGuiViewportPane is a friend of Scene
void ImGuiViewportPane::RenderScene(std::shared_ptr<Framebuffer> fb) {
  RenderEntityNode(scene.scene_root, fb); //identity transformation
}

// Before cumulative transformation matrices just render the entities
void ImGuiViewportPane::RenderEntityNode(entt::entity node, std::shared_ptr<Framebuffer> fb, glm::mat4 transform) {
  if (node == entt::null)
    return;

  if (scene.HasComponent<ShapeRenderer>(node) && scene.HasComponent<Transform>(node)) {
    ShapeRenderer* sr = scene.GetComponent<ShapeRenderer>(node);
    Transform* trans = scene.GetComponent<Transform>(node);
    if (!sr || !trans) {
        return;
    }
    transform = glm::translate(transform, trans->position);
    transform = glm::scale(transform, trans->scale);
    transform = glm::rotate(transform, trans->rotation.x, glm::vec3(0, 0, 1));
    renderer.DrawQuad(transform, camera.GetView(), camera.GetProj(), fb);
  }

  Relationship* r = scene.GetComponent<Relationship>(node);
  for (int i = 0; i < r->children.size(); i++) {
    RenderEntityNode(r->children.at(i), fb, transform);
  }
}
}
