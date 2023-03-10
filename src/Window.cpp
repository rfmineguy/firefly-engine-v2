#include "../include/Window.hpp"
#include <iostream>
#include <imgui-src/imgui.h>
#include <imgui-src/backends/imgui_impl_glfw.h>
#include <imgui-src/backends/imgui_impl_opengl3.h>

namespace FF {
Window::Window(): Window("Window", 600, 600) {}

Window::Window(const std::string& title, int width, int height) {
  InitWindow(title, width, height);
  fb = std::make_unique<FF::Framebuffer>(width, height);
}

Window::~Window() {
  glfwDestroyWindow(window_ptr);
  glfwTerminate();
}

void Window::InitWindow(const std::string& title, int width, int height) {
  if (!glfwInit()) {
    std::cerr << "FFError: Failed to initialize glfw" << std::endl;
    glfwTerminate();
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  window_ptr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window_ptr) {
    std::cerr << "FFError: Failed to create glfw window" << std::endl;
    glfwTerminate();
  }
  
  glfwMakeContextCurrent(window_ptr);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "FFError: Failed to initialize glad" << std::endl;
    glfwTerminate();
  }
  glViewport(0, 0, 600, 600);
  glfwSetFramebufferSizeCallback(window_ptr, framebuffer_size_callback);
}

bool Window::ShouldClose() {
  glfwPollEvents();
  glfwSwapBuffers(window_ptr);
  return glfwWindowShouldClose(window_ptr);
}

void Window::CloseWindow() {
  glfwSetWindowShouldClose(window_ptr, GLFW_TRUE);
}

std::shared_ptr<FF::Framebuffer> Window::GetFramebuffer() {
  return fb;
}

void Window::error_callback(int error, const char* description) {
  std::cerr << "GLFWError: " << description << std::endl;
}

void Window::framebuffer_size_callback(GLFWwindow* windowPtr, int width, int height) {
  glViewport(0, 0, width, height);
}
}