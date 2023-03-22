### Firefly Engine V2
This project is a second attempt at making a game engine

## Notes
- As of now for linux `libgtk-3-dev` is a known dependency
- You will need to layout the imgui widgets manually for now (just lay the widgets out in any way that makes sense for you)

## Used Libraries
- [glfw](https://github.com/glfw/glfw.git)
- [glad](https://github.com/Dav1dde/glad.git)
- [glm](https://github.com/g-truc/glm.git)
- [spdlog v1.x](https://github.com/gabime/spdlog.git)
- [imgui docking](https://github.com/ocornut/imgui.git)
- [imguizmo](https://github.com/CedricGuillemet/ImGuizmo.git)
- [EnTT](https://github.com/skypjack/entt.git)
- [nfd_extended v1.0.1](https://github.com/btzy/nativefiledialog-extended.git)
- [nlohmann-json v3.11.2](https://github.com/nlohmann/json.git)

## "Shoutouts"
- [Yaml Viewer](https://jsonformatter.org/yaml-viewer) was an indispensable tool in figuring out how to parse the yaml format 

## Build Instructions
```shell
mkdir build
cd build
cmake ..
make
```
*NOTE* The executable file is named `ff_engine`