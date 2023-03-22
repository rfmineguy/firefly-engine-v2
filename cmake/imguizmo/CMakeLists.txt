cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
add_library(imguizmo
  ImGuizmo.cpp)

message("imguizmo compile ${imgui_SOURCE_DIR}")
target_include_directories(imguizmo PRIVATE ${imgui_SOURCE_DIR}/)
