#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "../include/Framebuffer.hpp"
#include "../include/Components.hpp"

namespace FF {
class Renderer {
public:
  Renderer();
  ~Renderer();
  void ClearColor(int, int, int);
  
  void DrawShape(const Transform&, const ShapeRenderer&);
  void DrawSprite(const Transform&, const SpriteRenderer&);
  
  void SetTargetFramebuffer(std::shared_ptr<FF::Framebuffer>);

private:
  
private:
  std::weak_ptr<FF::Framebuffer> fb;
};
}

#endif