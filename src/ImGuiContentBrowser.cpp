#include "../include/ImGuiContentBrowser.hpp"
#include <filesystem>

namespace FF {
ImGuiContentBrowserPane::ImGuiContentBrowserPane(Project& p)
:ImGuiPane("ContentBrowser"), project(p) {}  

ImGuiContentBrowserPane::~ImGuiContentBrowserPane() {}

void ImGuiContentBrowserPane::Show(Window& window) {
  if (!visible)
    return;
  ImGui::Begin(name.c_str());

  if (!project.is_open) {
    ImGui::Text("No open project");
  }
  else {
    ImGui::Text("%s", project.root_path.c_str());
    for (const auto& entry : std::filesystem::recursive_directory_iterator(project.root_path)) {
      if (entry.is_regular_file()) {
        ImGui::Text("File %s", entry.path().c_str());
      }
      else {
        ImGui::Text("Dir: %s", entry.path().c_str());
      }
    }
  }

  ImGui::End();
}
}
