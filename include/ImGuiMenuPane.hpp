#ifndef IMGUI_MENU_PANE_HPP
#define IMGUI_MENU_PANE_HPP
#include "ImGuiPane.hpp"
#include "Scene.hpp"

namespace FF {
class ImGuiMenuPane : public ImGuiPane {
public:
  ImGuiMenuPane(Scene&, std::unordered_map<std::string, ImGuiPane*>&);
  ~ImGuiMenuPane();
  virtual void Show(Window&);

private:
  void ProcessOpenScene();

private:
  Scene& scene;
  std::unordered_map<std::string, ImGuiPane*>& panes;
};
}

#endif