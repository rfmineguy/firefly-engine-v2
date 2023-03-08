#include <iostream>
#include <spdlog-src/include/spdlog/spdlog.h>
#include "../include/Window.hpp"
#include "../include/Scene.hpp"
#include "../include/EngineSettings.hpp"
#include "../include/ImGuiLayer.hpp"

void scene_test() {
  FF::Scene scene;
  scene.NewEntity("Entity1");
  scene.NewEntity("Entity2");
  scene.Traverse();
}

int main() {
  FF::Window window;
  FF::ImGuiLayer::ImGuiInitialize(window);

  while (!window.ShouldClose()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Game world rendering
    //  ...
    
    // ImGui Rendering
    FF::ImGuiLayer::BeginFrame();
    FF::ImGuiLayer::BeginDockspace(window);

    FF::ImGuiLayer::ShowMainMenuBar(window);
    FF::ImGuiLayer::ShowRegisteredPanes(window);

    FF::ImGuiLayer::EndDockspace();
    FF::ImGuiLayer::EndFrame();
  }
}