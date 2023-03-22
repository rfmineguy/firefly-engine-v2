#ifndef IMGUI_INSPECTOR_PANE_HPP
#define IMGUI_INSPECTOR_PANE_HPP
#include "ImGuiPane.hpp"
#include "Scene2.hpp"

namespace FF {
class ImGuiInspectorPane: public ImGuiPane {
public:
  ImGuiInspectorPane(FF::Scene2&);
  ~ImGuiInspectorPane();
  virtual void Show(FF::Window&);

private:
  FF::Scene2& scene;
};
}

#endif