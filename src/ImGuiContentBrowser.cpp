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
    if (project.open_directory != project.root_path) {
      if (ImGui::Button("<-")) {
        project.open_directory = project.open_directory.parent_path();
      }
    }
    ImGui::Text("%s", project.open_directory.c_str());
    for (const auto& entry : std::filesystem::recursive_directory_iterator(project.open_directory)) {
      if (entry.is_regular_file()) {
        ImGui::Text("%s", entry.path().c_str());
      }
      else {
        if (ImGui::Button(entry.path().c_str())) {
          project.open_directory /= entry.path();
        }
      }
    }
  }

  ImGui::End();
}
}
