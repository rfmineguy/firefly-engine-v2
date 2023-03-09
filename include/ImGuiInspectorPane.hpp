#ifndef IMGUI_INSPECTOR_PANE_HPP
#define IMGUI_INSPECTOR_PANE_HPP
#include "ImGuiPane.hpp"
#include "Scene.hpp"

namespace FF {
class ImGuiInspectorPane: public ImGuiPane {
public:
  ImGuiInspectorPane(FF::Scene&);
  ~ImGuiInspectorPane();
  virtual void Show(FF::Window&);

private:
  FF::Scene& scene;
};
}

#endif