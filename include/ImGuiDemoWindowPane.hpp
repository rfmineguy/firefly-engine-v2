#ifndef IMGUI_DEMOWINDOWPANE_HPP
#define IMGUI_DEMOWINDOWPANE_HPP
#include "ImGuiPane.hpp"

class ImGuiDemoWindowPane : public ImGuiPane {
public:
  ImGuiDemoWindowPane();
  ~ImGuiDemoWindowPane();
  virtual void Show(FF::Window&);
};

#endif