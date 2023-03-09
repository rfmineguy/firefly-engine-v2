#include "../include/ImGuiDemoWindowPane.hpp"

ImGuiDemoWindowPane::ImGuiDemoWindowPane()
:ImGuiPane("Demo Window Pane") {
  visible = false;
}

ImGuiDemoWindowPane::~ImGuiDemoWindowPane() {
  
}

void ImGuiDemoWindowPane::Show(FF::Window& window) {
  if (!visible)
    return;
  ImGui::ShowDemoWindow();
}