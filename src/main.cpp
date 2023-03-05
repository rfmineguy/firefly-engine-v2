#include <iostream>
#include "../include/Window.hpp"
#include "../include/Scene.hpp"
#include "../include/EngineSettings.hpp"
#include "../include/ImGuiLayer.hpp"

int main() {
  // FF_Int::EngineSettings::Get();
  // FF::Window window;
  FF::Scene scene;
  FF::Entity e1 = scene.NewEntity("Entity1");
  FF::Entity e2 = scene.NewEntity("Entity2");
  scene.Traverse();
  
  try {
    FF::Entity e = scene.FindEntity("Entity1");
    std::cout << e.GetName() << std::endl;
  }
  catch (std::string e) {
    std::cerr << e << std::endl;
  }
  
//   FF::ImGuiLayer::ImGuiInitialize(window);
//   while (!window.ShouldClose()) {
//     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT);
//     
//     
//     
//     // ImGui Rendering
//     FF::ImGuiLayer::BeginFrame();
//     FF::ImGuiLayer::BeginDockspace(window);
// 
//     FF::ImGuiLayer::ShowMainMenuBar(window);
//     FF::ImGuiLayer::ShowRegisteredPanes(window);
// 
//     FF::ImGuiLayer::EndDockspace();
//     FF::ImGuiLayer::EndFrame();
//   }
}