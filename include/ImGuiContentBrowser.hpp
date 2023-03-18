#ifndef IMGUI_CONTENTBROWSER_PANE
#define IMGUI_CONTENTBROWSER_PANE
#include "ImGuiPane.hpp"
#include "Project.hpp"
#include "Texture.hpp"

namespace FF {
class ImGuiContentBrowserPane : public ImGuiPane {
public:
  ImGuiContentBrowserPane(Project&);
  ~ImGuiContentBrowserPane();
  virtual void Show(Window&);

private:
  Project& project;
  Texture file_icon, folder_icon;
};
}


#endif