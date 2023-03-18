#ifndef PROJECT_HPP
#define PROJECT_HPP
#include <string>
#include <filesystem>

namespace FF {
class Project {
public:
  Project();
  ~Project();

public:
  void Create(const std::string& path);
  void Save(const std::string& path);
  void SaveAs(const std::string& path);
  void Open(const std::string& path);
  void Close();

private:
  void Serialize();
  void Deserialize();

private:
  std::filesystem::path root_path;
  std::filesystem::path open_directory;
  bool is_open;

friend class ImGuiContentBrowserPane;
};
}

#endif