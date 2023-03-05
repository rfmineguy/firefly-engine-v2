#include <iostream>
#include "../include/Window.hpp"
#include "../include/EngineSettings.hpp"
#include "../include/ImGuiLayer.hpp"

int main() {
  FF_Int::EngineSettings::Get();
  FF::Window window;
  
  FF::ImGuiLayer::ImGuiInitialize(window);
  while (!window.ShouldClose()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    FF::ImGuiLayer::BeginFrame();
    ImGui::ShowDemoWindow();
    FF::ImGuiLayer::EndFrame();
  }
}