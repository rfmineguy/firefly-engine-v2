#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <memory>
#include <unordered_map>
#include "../include/Framebuffer.hpp"
#include "../include/Components.hpp"
#include "../include/Geometry.hpp"
#include "../include/Shader.hpp"

namespace FF {
class Renderer {
public:
  Renderer();
  ~Renderer();
  void ClearColor(int, int, int);
  
  void DrawQuad();
  void DrawQuad(glm::mat4);
  
  void SetTargetFramebuffer(std::shared_ptr<FF::Framebuffer>);

private:
  void UpdateProjectionMatrix(int, int, int, int);

private:
  // store RECTANGLE, TRIANGLE, CIRCLE, POLYGON
  FF::Geometry* geometry[ShapeRenderer::SHAPE_COUNT];
  std::unordered_map<std::string, std::unique_ptr<FF::Shader>> shaders;
  glm::mat4 projection;
  glm::mat4 view;
  
private:
  std::weak_ptr<FF::Framebuffer> fb;

friend class ImGuiViewportPane;
};
}

#endif