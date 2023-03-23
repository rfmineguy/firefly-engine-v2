#include <iostream>
#include <iomanip>
#include <spdlog-src/include/spdlog/spdlog.h>
#include "../include/Window.hpp"
#include "../include/Scene.hpp"
#include "../include/Scene2.hpp"
#include "../include/Logger.hpp"
#include "../include/ImGuiLayer.hpp"
#include "../include/Framebuffer.hpp"
#include "../include/Renderer.hpp"
#include "../include/Geometry.hpp"
#include "../include/Shader.hpp"

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
  
  FF_LOG_INFO("Deleting Entity1");
  scene.DeleteEntity(e1);
  FF_LOG_INFO("Deleted Entity1");
  scene.Traverse();
}

void scene2_test() {
  std::cout << "-==================================-" << std::endl;
  std::cout << "          Testing scene2 " << std::endl;
  std::cout << "-==================================-" << std::endl;
  FF::Scene2 scene2;
  entt::entity e0 = scene2.NewEntity("Entity0");
  entt::entity e1 = scene2.NewEntity("Entity1", e0);
  entt::entity e2 = scene2.NewEntity("Entity2", e1);
  entt::entity e3 = scene2.NewEntity("Entity3");

  if (scene2.IsChildOfRec(e3, e0)) {
    std::cout << "e3 is a child of e0" << std::endl;
  }
  else {
    std::cout << "e3 is not a child of e0" << std::endl;
  }
  scene2.Traverse();
}

void scene_serialize_deserialize_test() {
  std::cout << "-==================================-" << std::endl;
  std::cout << "    Testing serializing to yaml " << std::endl;
  std::cout << "-==================================-" << std::endl;
  FF::Scene scene;
  scene.NewEntity("Entity1");
  scene.NewEntity("Entity2");
  FF::Entity* e3 = scene.NewEntity("Entity3");
  FF::Entity* e4 = e3->AddChild(scene.NewEntity("Entity4"));
  e4->AddChild(scene.NewEntity("Entity5"));
  e4->AddChild(scene.NewEntity("Entity6"));
  FF::Entity* e7 = e3->AddChild(scene.NewEntity("Entity7"));
  e7->AddChild(scene.NewEntity("Entity8"));
  scene.SerializeToFile("data/test_serialize/scene2.yaml");

  std::cout << "-==================================-" << std::endl;
  std::cout << "  Testing deserializing scene file " << std::endl;
  std::cout << "-==================================-" << std::endl;
  FF::Scene scene2;
  scene2.DeserializeFromFile("data/test_serialize/scene2.yaml");
  scene2.Traverse();

  FF::Entity* e72 = scene2.FindEntityNode("Entity7");
  e72->AddChild(scene2.NewEntity("Entity9"));
  scene2.SerializeToFile("data/test_serialize/scene3.json");
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
// #define SCENE2_TEST
// #define SCENE_SERIALIZE_DESERIALIZE_TEST
// #define FRAMEBUFFER_TEST
// #define GEOMETRY_TEST
// #define SHADER_TEST
// #define LOGGER_TEST

void LaunchLauncher() {
  FF::Window window("Firefly Launcher", 600, 600);
  FF::ImGuiLayer::ImGuiInitialize(window);

  while (!window.ShouldClose()) {
    FF::ImGuiLayer::BeginFrame();
    FF::ImGuiLayer::BeginDockspace(window);

    ImGui::Begin("Recents");
    ImGui::Text("Firefly Engine Launcher");

    ImGui::End();

    ImGui::Begin("Main");
    ImGui::Button("New Project");
    ImGui::Button("Open Project");
    ImGui::End();
    
    FF::ImGuiLayer::EndDockspace();
    FF::ImGuiLayer::EndFrame();
  }

  FF::ImGuiLayer::ImGuiDeinitialize();
}

void LaunchEngine() {
  FF::Window window("Firefly Engine", 1000, 1000);
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
}

int main() {
#ifdef SCENE_TEST
  scene_test();
#elifdef SCENE2_TEST
  scene2_test();
#elifdef SCENE_SERIALIZE_DESERIALIZE_TEST
  scene_serialize_deserialize_test();
#elifdef FRAMEBUFFER_TEST
  framebuffer_test();
#elifdef GEOMETRY_TEST
  geometry_test();
#elifdef SHADER_TEST
  shader_test();
#elifdef LOGGER_TEST
  logger_test();
#else
  // LaunchLauncher();
  LaunchEngine();
#endif
}