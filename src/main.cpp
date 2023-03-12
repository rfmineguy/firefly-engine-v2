#include <iostream>
#include <iomanip>
#include <spdlog-src/include/spdlog/spdlog.h>
#include "../include/Window.hpp"
#include "../include/Scene.hpp"
#include "../include/Logger.hpp"
#include "../include/ImGuiLayer.hpp"
#include "../include/Framebuffer.hpp"
#include "../include/Renderer.hpp"
#include "../include/Geometry.hpp"
#include "../include/Shader.hpp"

/**
NOTE: NOTE: NOTE: NOTE:
Deleting entity with gui
  - Instead of trying to delete the entity while the heirarchy panel is
    rendering it. Set a flag in the entity(mark it dirty) that signifies whether
    it should be deleted after rendering in imgui.
**/

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
  // FF::Entity* d = test2->RemoveChild(scene.FindEntityNode("Test9"));
  
  scene.Traverse();
  
  FF_LOG_INFO("Deleting Entity1");
  scene.DeleteEntity(e1);
  FF_LOG_INFO("Deleted Entity1");
  scene.Traverse();
}

void framebuffer_test() {
  std::cout << "-==================================-" << std::endl;
  std::cout << "      Testing the framebuffer" << std::endl;
  std::cout << "-==================================-" << std::endl;
  FF::Window w;
}

void geometry_test() {
  std::cout << "-==================================-" << std::endl;
  std::cout << "      Testing the geometry" << std::endl;
  std::cout << "-==================================-" << std::endl;
  FF::Window w;
  FF::Geometry& g = FF::Geometry::Quad();
  std::cout << g.vertices.size() << std::endl;
  std::cout << g.indices.size() << std::endl;
  
  FF::Geometry& g2 = FF::Geometry::Quad();
  std::cout << g2.vertices.size() << std::endl;
  std::cout << g2.indices.size() << std::endl;
}

void shader_test() {
  FF::Window w;
  FF::Shader s("res/test.vert", "res/test.frag");
}

void logger_test() {
  FF_LOG_INFO("This is the first log");
}

// #define SCENE_TEST
// #define FRAMEBUFFER_TEST
// #define GEOMETRY_TEST
// #define SHADER_TEST
// #define LOGGER_TEST

int main() {
#ifdef SCENE_TEST
  scene_test();
#elifdef FRAMEBUFFER_TEST
  framebuffer_test();
#elifdef GEOMETRY_TEST
  geometry_test();
#elifdef SHADER_TEST
  shader_test();
#elifdef LOGGER_TEST
  logger_test();
#else
  FF::Window window;
  FF::ImGuiLayer::ImGuiInitialize(window);

  while (!window.ShouldClose()) {
    // ImGui Rendering
    FF::ImGuiLayer::BeginFrame();
    FF::ImGuiLayer::BeginDockspace(window);

    FF::ImGuiLayer::ShowRegisteredPanes(window);

    FF::ImGuiLayer::EndDockspace();
    FF::ImGuiLayer::EndFrame();
  }
  
  FF::ImGuiLayer::ImGuiDeinitialize(); // Important
#endif
}