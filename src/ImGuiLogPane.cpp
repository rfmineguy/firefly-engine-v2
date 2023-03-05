#include "../include/ImGuiLogPane.hpp"

ImGuiLogPane::ImGuiLogPane(): ImGuiPane("Log") {}
ImGuiLogPane::~ImGuiLogPane() {}
void ImGuiLogPane::Show(FF::Window& window) {
  if (!visible)
    return;
  ImGui::Begin(name.c_str());
  ImGui::End();
}