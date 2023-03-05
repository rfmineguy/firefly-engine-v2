#ifndef IMGUI_LAYER_HPP
#define IMGUI_LAYER_HPP
#include "Window.hpp"
#include "ImGuiPane.hpp"
#include <imgui-src/imgui.h>
#include <unordered_map>

namespace FF {
class ImGuiLayer {
public:
  static void ImGuiInitialize(FF::Window&);
  static void ImGuiDeinitialize();
  
  static void BeginFrame();
  static void EndFrame();
  
  static void BeginDockspace(FF::Window&);
  static void EndDockspace();
  
  static void ShowMainMenuBar(FF::Window&);
  static void ShowRegisteredPanes(FF::Window&);
  
protected:
  static void RegisterPanes();
public:
  static std::unordered_map<std::string, ImGuiPane*> panes;
};
}

#endif