#include "../include/ImGuiMenuPane.hpp"
#include "../include/Logger.hpp"

namespace FF {

ImGuiMenuPane::ImGuiMenuPane(Scene2& scene, Project& project, std::unordered_map<std::string, ImGuiPane*>& panes)
:ImGuiPane("MenuPane"), scene(scene), project(project), panes(panes), nfdGuard() {
}

ImGuiMenuPane::~ImGuiMenuPane() {
}

void ImGuiMenuPane::Show(Window& window) {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::Button("New Project")) {
        NFD::UniquePath outpath;
        nfdresult_t result = NFD::PickFolder(outpath);
        if (result == NFD_OKAY) {
          FF_LOG_DEBUG("[NFD] Create new project at directory: {}", outpath.get());
          try {
            project.Create(outpath.get());
          } catch (std::string& err) {
            FF_LOG_ERROR("{}", err);
          }
        }
        else if (result == NFD_CANCEL) {
          FF_LOG_DEBUG("[NFD] Canceled");
        }
        else {
          FF_LOG_ERROR("[NFD] {}", NFD_GetError());
        }
      }
      if (ImGui::Button("Open Project")) {
        NFD::UniquePath outpath;
        nfdresult_t result = NFD::PickFolder(outpath);
        if (result == NFD_OKAY) {
          FF_LOG_INFO("[NFD] Open directory: {}", outpath.get());
          try {
            project.Open(outpath.get());
          } catch (std::string& err) {
            FF_LOG_ERROR("{}", err);
          }
        }
        else if (result == NFD_CANCEL) {
          FF_LOG_INFO("[NFD] Canceled");
        }
        else {
          FF_LOG_ERROR("[NFD] {}", NFD::GetError());
        }
      }
      if (ImGui::Button("Save Scene")) {
        NFD::UniquePath outpath;
        nfdresult_t result = NFD::SaveDialog(outpath);
        if (result == NFD_OKAY) {
          FF_LOG_INFO("[NFD] Save to directory: {}", outpath.get());
          // scene.SerializeToFile(outpath.get());
        }
        else if (result == NFD_CANCEL) {
          FF_LOG_INFO("[NFD] Canceled");
        }
        else {
          FF_LOG_ERROR("[NFD] {}", NFD::GetError());
        }
        
      }
      if (ImGui::Button("Open Scene (Unstable)")) {
        NFD::UniquePath outpath;
        nfdresult_t result = NFD::OpenDialog(outpath);
        if (result == NFD_OKAY) {
          FF_LOG_INFO("[NFD] Open directory: {}", outpath.get());
          // scene.DeserializeFromFile(outpath.get());
        }
        else if (result == NFD_CANCEL) {
          FF_LOG_INFO("[NFD] Canceled");
        }
        else {
          FF_LOG_ERROR("[NFD] {}", NFD::GetError());
        }
        
      }

      // Generates button, "Exit"
      if (ImGui::Button("Exit")) {
        window.CloseWindow();
      }
      ImGui::EndMenu();
    }

    // Generates menu option "Window"
    if (ImGui::BeginMenu("Window")) {
      if (ImGui::BeginMenu("Layout")) {
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Visibility")) {
        for (auto& i : panes) {
          ImGui::Checkbox(i.second->Name().c_str(), &i.second->Visible());
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  } 
}
}