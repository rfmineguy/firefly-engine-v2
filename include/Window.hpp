#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace FF {
class Window {
public:
  Window();
  Window(const std::string&, int, int);
  ~Window();
  
public:
  bool ShouldClose();
  void CloseWindow();
  friend GLFWwindow* GetWindowPtr(Window*);    //Used in ImGuiLayer.cpp

private:
  void InitWindow(const std::string&, int, int);
  static void error_callback(int, const char*);
  static void framebuffer_size_callback(GLFWwindow*, int, int);

private:
  GLFWwindow* window_ptr;
};
}

#endif