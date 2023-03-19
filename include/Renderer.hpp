#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <memory>
#include <unordered_map>
#include "Components.hpp"
#include "Geometry.hpp"
#include "Shader.hpp"
#include "Framebuffer.hpp"

namespace FF {
class Renderer {
public:
  Renderer();
  ~Renderer();
  void ClearColor(float, float, float, float, std::shared_ptr<Framebuffer>);

  void DrawQuad();
  void DrawQuad(glm::mat4);
  void DrawQuad(glm::mat4, glm::vec4);

  void DrawQuad(glm::mat4 model, glm::mat4 view, glm::mat4 projection, std::shared_ptr<FF::Framebuffer>);
  
private:
  void UpdateProjectionMatrix(int, int, int, int);

private:
  // store RECTANGLE, TRIANGLE, CIRCLE, POLYGON
  FF::Geometry* geometry[ShapeRenderer::SHAPE_COUNT];
  std::unordered_map<std::string, std::unique_ptr<FF::Shader>> shaders;
  glm::mat4 projection;
  glm::mat4 view;
  
friend class ImGuiViewportPane;
};
}

#endif