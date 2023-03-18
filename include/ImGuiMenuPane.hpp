#ifndef IMGUI_MENU_PANE_HPP
#define IMGUI_MENU_PANE_HPP
#include "ImGuiPane.hpp"
#include "Scene.hpp"
#include "Project.hpp"
#include <nfd_extended-src/src/include/nfd.hpp>

namespace FF {
class ImGuiMenuPane : public ImGuiPane {
public:
  ImGuiMenuPane(Scene&, Project&, std::unordered_map<std::string, ImGuiPane*>&);
  ~ImGuiMenuPane();
  virtual void Show(Window&);

private:
  NFD::Guard nfdGuard;
  
private:
  Scene& scene;
  Project& project;
  std::unordered_map<std::string, ImGuiPane*>& panes;
};
}

#endif