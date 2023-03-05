#ifndef IMGUI_LAYER_HPP
#define IMGUI_LAYER_HPP
#include "Window.hpp"
#include <imgui-src/imgui.h>

namespace FF {
class ImGuiLayer {
public:
  static void ImGuiInitialize(FF::Window&);
  static void ImGuiDeinitialize();
  
  static void BeginFrame();
  static void EndFrame();
  
  static void BeginDockspace();
  static void EndDockspace();
};
}

#endif