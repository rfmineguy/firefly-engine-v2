#include "../include/Project.hpp"
#include "../include/Logger.hpp"
#include <fstream>

namespace FF {
Project::Project()
:is_open(false) {
  
}

Project::~Project() {
  
}

void Project::Create(const std::string& dirpath) {
  namespace fs = std::filesystem;
  
  fs::path path = dirpath;
  if (!fs::exists(path)) {
    fs::create_directory(path);
  }
  if (!fs::exists(path/"Scenes")) {
    fs::create_directory(path/"Scenes");
  }
  if (!fs::exists(path/"Assets")) {
    fs::create_directory(path/"Assets");
  }
  if (!fs::exists(path/".ffproj")) {
    std::ofstream of(path/".ffproj");
    of.close();
  }
  if (!fs::exists(path/".ffsettings")) {
    std::ofstream of(path/".ffsettings");
    of.close();
  }
  FF_LOG_INFO("Initialized project directory: {}", path.string());
  Open(dirpath);
}

void Project::Save(const std::string& dirpath) {
}

void Project::Open(const std::string& filepath) {
  std::filesystem::path path = filepath;
  if (!std::filesystem::exists(path)) {
    throw std::string("Directory does not exist");
  }
  if (!std::filesystem::exists(path / ".ffproj")) {
    throw std::string("Directory is not a firefly project directory");
  }
  FF_LOG_INFO("Directory is a valid firefly project");
  root_path = path;
  open_directory = path;
  is_open = true;
}

void Project::Serialize() {
  
}

void Project::Deserialize() {
  
}
}
