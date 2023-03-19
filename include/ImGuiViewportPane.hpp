#ifndef IMGUI_VIEWPORTPANE_HPP
#define IMGUI_VIEWPORTPANE_HPP
#include "ImGuiPane.hpp"
#include "Framebuffer.hpp"
#include "OrthographicCamera.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"

namespace FF {
class ImGuiViewportPane : public ImGuiPane {
public:
  ImGuiViewportPane(Scene&);
  ~ImGuiViewportPane();
  virtual void Show(Window&);

  // ImGuiViewportPane is a friend of Scene
private:
  void RenderScene(std::shared_ptr<Framebuffer>);
  void RenderEntityNode(Entity*, std::shared_ptr<Framebuffer>, glm::mat4 = glm::mat4(1.0));

private:
  FF::Renderer renderer;
  FF::OrthoCamera camera;
  ImVec2 last_viewport_size;
  FF::Scene& scene;
};
}

#endif