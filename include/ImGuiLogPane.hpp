#ifndef IMGUI_LOGPANE_HPP
#define IMGUI_LOGPANE_HPP
#include "ImGuiPane.hpp"

class ImGuiLogPane : public ImGuiPane {
public:
  ImGuiLogPane();
  ~ImGuiLogPane();
  virtual void Show(FF::Window&);  
};

#endif