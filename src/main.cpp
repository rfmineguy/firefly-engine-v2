#include <iostream>
#include <iomanip>
#include <spdlog-src/include/spdlog/spdlog.h>
#include "../include/Window.hpp"
#include "../include/Scene.hpp"
#include "../include/Logger.hpp"
#include "../include/EngineSettings.hpp"
#include "../include/ImGuiLayer.hpp"
#include "../include/Framebuffer.hpp"
#include "../include/Renderer.hpp"

void scene_test() {
  std::cout << "-==================================-" << std::endl;
  std::cout << "  Testing the scene data structure" << std::endl;
  std::cout << "-==================================-" << std::endl;

  FF::Scene scene;
  for (int i = 0; i < 10; i++) {
    scene.NewEntity("Entity" + std::to_string(i));
  }
  FF::Entity* e1 = scene.FindEntityNode("Entity1"); 
  FF::Entity* test = e1->AddChild(scene.NewEntity("Test"));
  FF::Entity* test2 = e1->AddChild(scene.NewEntity("Test2"));
  test2->AddChild(scene.NewEntity("Test9"));
  scene.Traverse();
}

void framebuffer_test() {
  std::cout << "-==================================-" << std::endl;
  std::cout << "      Testing the framebuffer" << std::endl;
  std::cout << "-==================================-" << std::endl;
  FF::Window w;
}

// #define SCENE_TEST
// #define FRAMEBUFFER_TEST

int main() {
#ifdef SCENE_TEST
  scene_test();
#elifdef FRAMEBUFFER_TEST
  framebuffer_test();
#else
  FF::Window window;
  FF::ImGuiLayer::ImGuiInitialize(window);
  // FF::Logger::Initialize();

  while (!window.ShouldClose()) {
    // ImGui Rendering
    FF::ImGuiLayer::BeginFrame();
    FF::ImGuiLayer::BeginDockspace(window);

    FF::ImGuiLayer::ShowMainMenuBar(window);
    FF::ImGuiLayer::ShowRegisteredPanes(window);

    FF::ImGuiLayer::EndDockspace();
    FF::ImGuiLayer::EndFrame();
  }
  
  FF::ImGuiLayer::ImGuiDeinitialize(); // Important
#endif
}