#include "../include/ImGuiLayer.hpp"
#include "../include/ImGuiViewportPane.hpp"
#include "../include/ImGuiLogPane.hpp"
#include "../include/ImGuiHeirarchyPane.hpp"
#include "../include/ImGuiDemoWindowPane.hpp"
#include <imgui-src/backends/imgui_impl_glfw.h>
#include <imgui-src/backends/imgui_impl_opengl3.h>
#include <iostream>

namespace FF {
  void ImGuiLayer::ImGuiInitialize(FF::Window& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(GetWindowPtr(&window), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    RegisterPanes();
  }

  void ImGuiLayer::ImGuiDeinitialize() {
    for (auto& i : Get().panes) {
      if (i.second == nullptr)
        continue;
      delete i.second;
      i.second = nullptr;
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiLayer::RegisterPanes() {
    Get().panes.emplace("demo", new ImGuiDemoWindowPane());
    Get().panes.emplace("viewport", new ImGuiViewportPane());
    Get().panes.emplace("log", new ImGuiLogPane());
    Get().panes.emplace("heirarchy", new ImGuiHeirarchyPane(Get().scene));
  }

  void ImGuiLayer::BeginFrame() {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
  
    // Enable docking for the next frame
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    ImGui::NewFrame();
  }

  void ImGuiLayer::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void ImGuiLayer::BeginDockspace(FF::Window& window) {
    static bool opt_fullscreen = true;
    static bool opt_is_open = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
  
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
      const ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus; 
    }
    else {
      dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;
    
    if (!opt_padding) {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    }
    ImGui::Begin("Dockspace Begin", &opt_is_open, window_flags);
    if (!opt_padding) {
      ImGui::PopStyleVar();
    }
    if (opt_fullscreen) {
      ImGui::PopStyleVar(2);
    }
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspace_id = ImGui::GetID("DockspaceID");
      ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), dockspace_flags);
    }
    else {
      // Docking is disabled
      // NOTE: Not sure how to recover or what to do here
    }
  }

  void ImGuiLayer::EndDockspace() {
    ImGui::End();
  }

  void ImGuiLayer::ShowMainMenuBar(FF::Window& window) {
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::Button("Exit")) {
          window.CloseWindow();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Window")) {
        if (ImGui::BeginMenu("Layout")) {
          
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Visibility")) {
          for (auto& i : Get().panes) {
            ImGui::Checkbox(i.second->Name().c_str(), &i.second->Visible());
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }

  void ImGuiLayer::ShowRegisteredPanes(FF::Window& window) {
    for (auto& i : Get().panes) {
      i.second->Show(window);
    }
  }

  // friend function of the Window class
  GLFWwindow* GetWindowPtr(Window* window) {
    return window->window_ptr;
  }
}