cmake_minimum_required(VERSION 3.20)
project(ff_engine)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

include(FetchContent)

# ==============================
# Get the GLFW windowing library
# ==============================
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
)
if(NOT glfw_POPULATED)
  FetchContent_Populate(glfw)

  set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
  set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
  set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
  set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

# ==============================
# Get the glad opengl loader
# ==============================
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
  FetchContent_Populate(glad)
  set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
  set(GLAD_API "gl=" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
  set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")
  message(${glad_BINARY_DIR})
  add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()

# ==============================
# Get the glm linear algebra library
# ==============================
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
)

FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
  FetchContent_Populate(glm)
  message(${glm_BINARY_DIR})
  message(${glm_SOURCE_DIR})
  add_subdirectory(${glm_SOURCE_DIR} $(glm_BINARY_DIR))
endif()

# ==============================
# Get the spdlog library
# ==============================
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
)

FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
  FetchContent_Populate(spdlog)
  message(${spdlog_SOURCE_DIR})
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif()

# ==============================
# Get the imgui library
# ==============================
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        docking
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
  file(COPY cmake/imgui/CMakeLists.txt DESTINATION ${imgui_SOURCE_DIR})
  include_directories(${glfw_SOURCE_DIR}/include/)
  add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR})
endif()

# ==============================
# Get the Entt library
# ==============================
FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt.git
)

FetchContent_GetProperties(entt)
if (NOT entt_POPULATED)
  FetchContent_Populate(entt)
  message(${entt_SOURCE_DIR})
  add_subdirectory(${entt_SOURCE_DIR} ${entt_BINARY_DIR})
endif()

add_executable(ff_engine
  src/main.cpp
  src/Entity.cpp
  src/Window.cpp
  src/Framebuffer.cpp
  src/Renderer.cpp
  src/ImGuiLayer.cpp
  src/ImGuiViewportPane.cpp
  src/ImGuiLogPane.cpp
  src/ImGuiHeirarchyPane.cpp
  src/ImGuiDemoWindowPane.cpp
  src/ImGuiInspectorPane.cpp
  src/Scene.cpp
  src/Geometry.cpp
)

target_link_libraries(ff_engine glfw glm glad spdlog imgui EnTT::EnTT)
target_include_directories(ff_engine PRIVATE ${spdlog_SOURCE_DIR}/include/spdlog ${glfw_SOURCE_DIR}/include/ ${glm_SOURCE_DIR}/ ${glad_SOURCE_DIR}/ ${imgui_SOURCE_DIR}/ ${entt_SOURCE_DIR}/single_include/)
