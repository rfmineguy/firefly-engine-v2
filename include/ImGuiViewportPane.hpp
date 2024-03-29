#ifndef IMGUI_VIEWPORTPANE_HPP
#define IMGUI_VIEWPORTPANE_HPP
#include "ImGuiPane.hpp"
#include "Framebuffer.hpp"
// #include "PerspectiveCamera.hpp"
#include "OrthographicCamera.hpp"
#include "Renderer.hpp"
#include "Scene2.hpp"
#include "Texture.hpp"

namespace FF {
class ImGuiViewportPane : public ImGuiPane {
public:
  ImGuiViewportPane(Scene2&);
  ~ImGuiViewportPane();
  virtual void Show(Window&);

  // ImGuiViewportPane is a friend of Scene
private:
  void RenderScene(std::shared_ptr<Framebuffer>);
  void RenderEntityNode(entt::entity, std::shared_ptr<Framebuffer>, glm::mat4 = glm::mat4(1.0));
  void RenderGizmos(Relationship*, Transform*, glm::mat4, glm::mat4, glm::mat4);

private:
  FF::Renderer renderer;
  FF::OrthoCamera camera;
  ImVec2 last_viewport_size;
  ImVec2 bottom_left;
  FF::Scene2& scene;
  int current_gizmo_operation;    // ImGuizmo::OPERATION
  int current_gizmo_mode;    // ImGuizmo::MODE
  
private:
  FF::Texture move_icon, eyeball_icon;
};
}

  #endif