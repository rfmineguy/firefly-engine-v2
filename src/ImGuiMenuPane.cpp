#include "../include/ImGuiMenuPane.hpp"
#include "../include/Logger.hpp"
#include <nfd_extended-src/src/include/nfd.h>

namespace FF {

ImGuiMenuPane::ImGuiMenuPane(Scene& scene, std::unordered_map<std::string, ImGuiPane*>& panes)
:ImGuiPane("MenuPane"), scene(scene), panes(panes) {
  if (NFD_Init() != NFD_OKAY) {
    FF_LOG_ERROR("NFD Error");
  }
  else {
    FF_LOG_INFO("NFD Initialized");
  }
}

ImGuiMenuPane::~ImGuiMenuPane() {
  NFD_Quit();
  FF_LOG_WARN("NFD Deinitialized");
}

void ImGuiMenuPane::Show(Window& window) {
  if (ImGui::BeginMenuBar()) {
    // Generates menu option "File"
    if (ImGui::BeginMenu("File")) {
      // Generates button, "Open"
      // ProcessOpenScene();
      if (ImGui::Button("New Project")) {
        nfdchar_t *outpath;
        nfdfilteritem_t filterItem[1] = { {  } };
        nfdresult_t result = NFD_OpenDialog(&outpath, filterItem, 1, NULL);
        if (result == NFD_OKAY) {
          FF_LOG_DEBUG("[NFD] New project at directory: {}", outpath);
          NFD_FreePath(outpath);
        }
        else if (result == NFD_CANCEL) {
          FF_LOG_DEBUG("[NFD] Canceled");
        }
        else {
          FF_LOG_ERROR("[NFD] {}", NFD_GetError());
        }
        
      }
      if (ImGui::Button("Open Project")) {
        nfdchar_t *outpath;
        nfdfilteritem_t filterItem[1] = { { "FF Project File", "ffprj" } };
        nfdresult_t result = NFD_OpenDialog(&outpath, filterItem, 1, NULL);
        if (result == NFD_OKAY) {
          FF_LOG_DEBUG("[NFD] Open directory: {}", outpath);
          NFD_FreePath(outpath);
        }
        else if (result == NFD_CANCEL) {
          FF_LOG_DEBUG("[NFD] Canceled");
        }
        else {
          FF_LOG_ERROR("[NFD] {}", NFD_GetError());
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
void ImGuiMenuPane::ProcessOpenScene() {
  if (ImGui::Button("Open Scene")) {
    nfdchar_t *outpath;
    nfdfilteritem_t filterItem[2] = { { "FF Scene File", "ffscn" } };
    nfdresult_t result = NFD_OpenDialog(&outpath, filterItem, 2, NULL);
    if (result == NFD_OKAY) {
      FF_LOG_DEBUG("[NFD] Open directory: {}", outpath);
      NFD_FreePath(outpath);
    }
    else if (result == NFD_CANCEL) {
      FF_LOG_DEBUG("[NFD] Canceled");
    }
    else {
      FF_LOG_ERROR("[NFD] {}", NFD_GetError());
    }
  }
}
}