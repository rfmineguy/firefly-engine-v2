#ifndef IMGUI_LAYER_HPP
#define IMGUI_LAYER_HPP
#include "Window.hpp"
#include "Scene.hpp"
#include "Project.hpp"
#include "ImGuiPane.hpp"
#include <imgui-src/imgui.h>
#include <unordered_map>

namespace FF {
class ImGuiLayer {
private:
  ImGuiLayer() {}
  ~ImGuiLayer() {}

public:
  static ImGuiLayer& Get() {
    static ImGuiLayer layer;
    return layer;
  }

public:
  static void ImGuiInitialize(FF::Window&);
  static void ImGuiDeinitialize();
  
  static void BeginFrame();
  static void EndFrame();
  
  static void BeginDockspace(FF::Window&);
  static void EndDockspace();
  
  static void ShowRegisteredPanes(FF::Window&);
  
protected:
  static void RegisterPanes();
  static void InitTheming();

public:
  std::unordered_map<std::string, ImGuiPane*> panes;
  FF::Scene scene;
  FF::Project project;
};
}

#endif