#ifndef IMGUI_PANE_HPP
#define IMGUI_PANE_HPP
#include "Window.hpp"
#include <imgui-src/imgui.h>

class ImGuiPane {
public:
  ImGuiPane(const std::string& title): visible(true), name(title) {}
  virtual ~ImGuiPane() {}
  
  virtual void Show(FF::Window&) = 0;
  bool& Visible() { return visible; }
  const std::string& Name() const { return name; }

protected:
  std::string name;
  bool visible;
};

#endif