#include "../include/ImGuiViewportPane.hpp"
#include "../include/Logger.hpp"
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

  camera.Update();

  std::shared_ptr<FF::Framebuffer> fb = window.GetFramebuffer();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin(name.c_str());
  ImGui::Text("Resize viewport to see entities rendered");
  
  // Handle viewport resizing
  ImVec2 viewport_size = ImGui::GetContentRegionAvail();
  ImVec2 viewport_pos = ImGui::GetWindowPos();
  static bool first = true;
  if (first) {
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

  renderer.ClearColor(0.6f, 0.6f, 0.6f, 1.f, fb);
  RenderScene(fb);
  
  ImGui::Image((void*)(intptr_t)fb->GetColorAttachment(), viewport_size);
  ImGui::GetForegroundDrawList()->AddCircle(ImGui::GetMousePos(), 10, IM_COL32(255, 0, 0, 255), 100, 1.0f);

  if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
    ImVec2 m_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    FF_LOG_INFO("Mouse dragging. Mouse delta dx: {} dy: {}", m_delta.x, m_delta.y);
  }

  ImGui::End();
  ImGui::PopStyleVar();
}

// ImGuiViewportPane is a friend of Scene
void ImGuiViewportPane::RenderScene(std::shared_ptr<Framebuffer> fb) {
  RenderEntityNode(scene.entity_tree_root, fb); //identity transformation
}

// Before cumulative transformation matrices just render the entities
void ImGuiViewportPane::RenderEntityNode(Entity* node, std::shared_ptr<Framebuffer> fb, glm::mat4 transform) {
  if (node == nullptr)
    return;

  if (node->HasComponent<ShapeRenderer>() && node->HasComponent<Transform>()) {
    ShapeRenderer* rs = node->GetComponent<ShapeRenderer>();
    Transform* t = node->GetComponent<Transform>();
    if (!rs || !t) {
        return;
    }
    transform = glm::translate(transform, t->position);
    transform = glm::scale(transform, t->scale);
    transform = glm::rotate(transform, t->rotation.x, glm::vec3(0, 0, 1));
    ImGui::GetForegroundDrawList()->AddCircle({t->position[0], t->position[1]}, 10, IM_COL32(255, 0, 0, 255), 100, 1.0f);
    renderer.DrawQuad(transform, camera.GetView(), camera.GetProj(), fb);
  }

  for (int i = 0; i < node->children.size(); i++) {
    RenderEntityNode(node->children.at(i), fb, transform);
  }
}
}
