#ifndef IMGUI_HEIRARCHYPANE_HPP
#define IMGUI_HEIRARCHYPANE_HPP
#include "ImGuiPane.hpp"
#include "Scene.hpp"

namespace FF {
class ImGuiHeirarchyPane : public ImGuiPane {
public:
  ImGuiHeirarchyPane(FF::Scene&);
  ~ImGuiHeirarchyPane();
  virtual void Show(FF::Window&);

private:
  // ImGuiHeirarchyPane is a friend of Scene
  void ShowHeirarchy(FF::Entity*);
  
  int ShowEntityNode(FF::Entity*);
  void ValidateTree(FF::Entity*);
  
public:
  FF::Scene& scene;
  
private:
  std::string main_selection_id, right_click_selection_id;
};
}

#endif