cmake_minimum_required(VERSION 3.20)
project(ff_engine)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

file (COPY res DESTINATION ${CMAKE_BINARY_DIR})
file (COPY data DESTINATION ${CMAKE_BINARY_DIR})

include(FetchContent)

# ==============================
# Library list
# ==============================
#   - glfw
#   - glad
#   - glm
#   - spdlog
#   - imgui    (docking)
#   - entt
#   - nfd-extended
#   - nlohmann-json (json parser, will replace yamlcpp)
#   - stb

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
  GIT_TAG v1.x
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

# ==============================
# Get the nfd-extended library
# ==============================
FetchContent_Declare(
  nfd_extended
  GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
  GIT_TAG v1.0.1
)

FetchContent_GetProperties(nfd_extended)
if (NOT nfd_extended_POPULATED)
  FetchContent_Populate(nfd_extended)
  message(${nfd_extended_SOURCE_DIR})
  add_subdirectory(${nfd_extended_SOURCE_DIR} ${nfd_extended_BINARY_DIR})
endif()

# ==============================
# Get the nlohmann_json library
# ==============================
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)

FetchContent_GetProperties(nlohmann_json)
if (NOT nlohmann_json_POPULATED)
  FetchContent_Populate(nlohmann_json)
  message(${nlohmann_json_SOURCE_DIR})
  add_subdirectory(${nlohmann_json_SOURCE_DIR})
endif()

# ==============================
# Get the stb library
# ==============================
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
)

FetchContent_GetProperties(stb)
if (NOT stb_POPULATED)
  FetchContent_Populate(stb)
  message(${stb_SOURCE_DIR})
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
  src/ImGuiMenuPane.cpp
  src/ImGuiContentBrowser.cpp
  src/Project.cpp
  src/Scene.cpp
  src/Geometry.cpp
  src/Texture.cpp
  src/Shader.cpp
  src/Logger.cpp
  src/stb_impl.cpp
)

target_link_libraries(ff_engine glfw glm glad spdlog imgui EnTT::EnTT nfd nlohmann_json)
target_include_directories(ff_engine PRIVATE 
  ${spdlog_SOURCE_DIR}/include/spdlog 
  ${glfw_SOURCE_DIR}/include/ 
  ${glm_SOURCE_DIR}/ ${glad_SOURCE_DIR}/
  ${imgui_SOURCE_DIR}/
  ${entt_SOURCE_DIR}/single_include/
  ${nfd-extended_SOURCE_DIR}/src/include/
)