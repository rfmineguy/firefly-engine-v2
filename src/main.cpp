#include <iostream>
#include <iomanip>
#include <spdlog-src/include/spdlog/spdlog.h>
#include "../include/Window.hpp"
#include "../include/Scene.hpp"
#include "../include/EngineSettings.hpp"
#include "../include/ImGuiLayer.hpp"

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

#define SCENE_TEST

int main() {
#ifdef SCENE_TEST
  scene_test();
#else
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
#endif
}