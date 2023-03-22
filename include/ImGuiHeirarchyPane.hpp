#ifndef IMGUI_HEIRARCHYPANE_HPP
#define IMGUI_HEIRARCHYPANE_HPP
#include "ImGuiPane.hpp"
#include "Scene2.hpp"

namespace FF {
class ImGuiHeirarchyPane : public ImGuiPane {
public:
  ImGuiHeirarchyPane(FF::Scene2&);
  ~ImGuiHeirarchyPane();
  virtual void Show(FF::Window&);

private:
  // ImGuiHeirarchyPane is a friend of Scene
  void ShowHeirarchy(entt::entity);
  int ShowEntityNode(entt::entity);
  
public:
  FF::Scene2& scene;
  
private:
  std::string main_selection_id, right_click_selection_id;
};
}

#endif