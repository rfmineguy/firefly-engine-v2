#include "../include/ImGuiContentBrowser.hpp"
#include "../include/Logger.hpp"
#include <filesystem>

namespace FF {
ImGuiContentBrowserPane::ImGuiContentBrowserPane(Project& p)
:ImGuiPane("ContentBrowser"), project(p), 
file_icon("res/textures/icons/icon_file.png"),
folder_icon("res/textures/icons/icon_folder.png") {}  

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
    if (ImGui::BeginTable("ContentBrowser", 4)) {
      int id = 0;
      for (const auto& entry : std::filesystem::directory_iterator(project.open_directory)) {
        if (entry.is_regular_file()) {
          ImGui::PushID(id);
          if (ImGui::ImageButton((void*)(intptr_t) file_icon.Handle(), {64, 64}, {0, 1}, {1, 0})) {
            FF_LOG_INFO("Open file");
          }
          ImGui::PopID();
        }
        else {
          ImGui::PushID(id);
          if (ImGui::ImageButton((void*)(intptr_t) folder_icon.Handle(), {64, 64}, {0, 1}, {1, 0})) {
             project.open_directory /= entry.path();
          }
          ImGui::PopID();
        }  
        ImGui::Text("%s", entry.path().filename().c_str());
        ImGui::TableNextColumn();
        id ++;
      }
      ImGui::EndTable();
    }
  }
    
  ImGui::End();
}
}
