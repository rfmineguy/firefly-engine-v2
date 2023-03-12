#include "../include/ImGuiMenuPane.hpp"
#include "../include/Logger.hpp"
#include <nfd_extended-src/src/include/nfd.h>

namespace FF {

ImGuiMenuPane::ImGuiMenuPane(Scene& scene, std::unordered_map<std::string, ImGuiPane*>& panes)
:ImGuiPane("MenuPane"), scene(scene), panes(panes) {
  if (NFD_Init() != NFD_OKAY) {
    FF_LOG_ERROR("NFD Error");
  }
}

ImGuiMenuPane::~ImGuiMenuPane() {
  NFD_Quit();
}

void ImGuiMenuPane::Show(Window& window) {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::Button("Open")) {
        nfdchar_t *outpath;
        nfdfilteritem_t filterItem[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp"} };
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