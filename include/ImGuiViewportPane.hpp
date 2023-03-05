#ifndef IMGUI_VIEWPORTPANE_HPP
#define IMGUI_VIEWPORTPANE_HPP
#include "ImGuiPane.hpp"

class ImGuiViewportPane : public ImGuiPane {
public:
  ImGuiViewportPane();
  ~ImGuiViewportPane();
  virtual void Show(FF::Window&);
};

#endif