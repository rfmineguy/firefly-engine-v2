#include "../include/ImGuiLayer.hpp"
#include <imgui-src/backends/imgui_impl_glfw.h>
#include <imgui-src/backends/imgui_impl_opengl3.h>

namespace FF {
  void ImGuiLayer::ImGuiInitialize(FF::Window& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(GetWindowPtr(&window), true);
    ImGui_ImplOpenGL3_Init("#version 330");
  }

  void ImGuiLayer::ImGuiDeinitialize() {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiLayer::BeginFrame() {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
  }

  void ImGuiLayer::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void ImGuiLayer::BeginDockspace() {
  
  }

  void ImGuiLayer::EndDockspace() {
  
  }

  // friend function of the Window class
  GLFWwindow* GetWindowPtr(Window* window) {
    return window->window_ptr;
  }
}