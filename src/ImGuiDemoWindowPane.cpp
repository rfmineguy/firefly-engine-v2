#include "../include/ImGuiDemoWindowPane.hpp"

ImGuiDemoWindowPane::ImGuiDemoWindowPane()
:ImGuiPane("Demo Window Pane") {}

ImGuiDemoWindowPane::~ImGuiDemoWindowPane() {
  
}

void ImGuiDemoWindowPane::Show(FF::Window& window) {
  if (!visible)
    return;
  ImGui::ShowDemoWindow();
}