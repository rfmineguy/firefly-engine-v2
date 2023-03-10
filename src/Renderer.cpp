#include "../include/Renderer.hpp"
#include <glad/glad.h>
#include <iostream>
#include <glm-src/glm/gtc/matrix_transform.hpp>

#define FB_ASSERT_VALID(fb) \
  if (fb == nullptr) \
    std::cerr << "Active framebuffer is invalid" << std::endl; \
    return \

namespace FF {
Renderer::Renderer(): 
  shader("res/test.vert", "res/test.frag")  {}

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
  fb.lock()->Bind();
  q.Bind();
  shader.Bind();
  shader.SetUniformMat4("model", glm::mat4(1.0f));
  shader.SetUniformMat4("view", glm::mat4(1.0f));
  shader.SetUniformMat4("proj", projection);
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  
  shader.Unbind();
  q.Unbind();
  fb.lock()->Unbind();
}

void Renderer::DrawQuad(glm::mat4 transform) {
  if (fb.expired())
    return;

  FF::Geometry& q = FF::Geometry::Quad();
  fb.lock()->Bind();
  q.Bind();

  shader.Bind();
  shader.SetUniformMat4("model", transform);
  shader.SetUniformMat4("view", glm::mat4(1.0f));
  shader.SetUniformMat4("proj", projection);
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  
  shader.Unbind();
  q.Unbind();
  fb.lock()->Unbind();
}

void Renderer::DrawShape(const Transform& t, const ShapeRenderer& shapeRenderer) {
  if (fb.expired())
    return;
  FF::Geometry& q = FF::Geometry::Quad();
  fb.lock()->Bind();
  q.Bind();
  shader.Bind();
  shader.SetUniformMat4("model", glm::mat4(1.0f));
  shader.SetUniformMat4("view", glm::mat4(1.0f));
  shader.SetUniformMat4("proj", projection);
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  shader.Unbind();
  q.Unbind();
  fb.lock()->Unbind();
}

void Renderer::DrawSprite(const Transform& t, const SpriteRenderer& spriteRenderer) {
  if (fb.expired())
    return;
}

void Renderer::SetTargetFramebuffer(std::shared_ptr<FF::Framebuffer> _fb) {
  fb = _fb;
}

void Renderer::UpdateProjectionMatrix(int newW, int newH) {
  std::cout << "Update Proj: " << newW << " x " << newH << std::endl;
  projection = glm::ortho(0.f, (float)newW, (float)newH, 0.f, 0.f, 1000.f);
}
}
