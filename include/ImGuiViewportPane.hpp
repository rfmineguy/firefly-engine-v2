#ifndef IMGUI_VIEWPORTPANE_HPP
#define IMGUI_VIEWPORTPANE_HPP
#include "ImGuiPane.hpp"
#include "Framebuffer.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"

class ImGuiViewportPane : public ImGuiPane {
public:
  ImGuiViewportPane(FF::Scene&);
  ~ImGuiViewportPane();
  virtual void Show(FF::Window&);

  // ImGuiViewportPane is a friend of Scene
  void RenderScene();
  
private:
  FF::Renderer renderer;
  ImVec2 last_viewport_size;
  FF::Scene& scene;
};

#endif