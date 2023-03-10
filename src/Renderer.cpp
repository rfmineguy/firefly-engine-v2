#include "../include/Renderer.hpp"
#include <glad/glad.h>
#include <iostream>

#define FB_ASSERT_VALID(fb) \
  if (fb == nullptr) \
    std::cerr << "Active framebuffer is invalid" << std::endl; \
    return \

namespace FF {
Renderer::Renderer()  {}

Renderer::~Renderer() {}

void Renderer::ClearColor(int r, int g, int b) {
  if (fb.expired())
    return;

  fb.lock()->Bind();
  glClearColor(r / 255.f, g / 255.f, b / 255.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  fb.lock()->Unbind();
}

void Renderer::DrawQuad() {
  if (fb.expired())
    return;
  FF::Geometry& q = FF::Geometry::Quad();
  q.Bind();
  // TODO: I need a shader in order to draw anything
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  q.Unbind();
}

void Renderer::DrawShape(const Transform& t, const ShapeRenderer& shapeRenderer) {
  if (fb.expired())
    return;
}

void Renderer::DrawSprite(const Transform& t, const SpriteRenderer& spriteRenderer) {
  if (fb.expired())
    return;
}

void Renderer::SetTargetFramebuffer(std::shared_ptr<FF::Framebuffer> _fb) {
  fb = _fb;
}

}
