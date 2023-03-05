#ifndef IMGUI_HEIRARCHYPANE_HPP
#define IMGUI_HEIRARCHYPANE_HPP
#include "ImGuiPane.hpp"
#include "Scene.hpp"

class ImGuiHeirarchyPane : public ImGuiPane {
public:
  ImGuiHeirarchyPane();
  ~ImGuiHeirarchyPane();
  virtual void Show(FF::Window&);
  
public:
  void ShowSceneNode(FF::Scene::Node*);
};

#endif