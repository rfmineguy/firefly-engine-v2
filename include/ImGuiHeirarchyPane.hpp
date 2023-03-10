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
  bool ShowEntityNode(FF::Entity*);
  
public:
  FF::Scene& scene;
  
private:
  std::string selection_id;
};
}

#endif