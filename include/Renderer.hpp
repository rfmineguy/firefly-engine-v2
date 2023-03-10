#ifndef RENDERER_HPP
#define RENDERER_HPP
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
  void DrawShape(const Transform&, const ShapeRenderer&);
  void DrawSprite(const Transform&, const SpriteRenderer&);
  
  void SetTargetFramebuffer(std::shared_ptr<FF::Framebuffer>);

private:
  // store RECTANGLE, TRIANGLE, CIRCLE, POLYGON
  FF::Geometry* geometry[ShapeRenderer::SHAPE_COUNT];
  FF::Shader shader;
  
private:
  std::weak_ptr<FF::Framebuffer> fb;
};
}

#endif