#include "../include/ImGuiViewportPane.hpp"

ImGuiViewportPane::ImGuiViewportPane(): ImGuiPane("Viewport") {}
ImGuiViewportPane::~ImGuiViewportPane() {}
void ImGuiViewportPane::Show(FF::Window& window) {
  if (!visible)
    return;
  ImGui::Begin(name.c_str());
  ImGui::End();
}