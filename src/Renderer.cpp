#include "../include/Renderer.hpp"
#include "../include/Logger.hpp"
#include <glad/glad.h>
#include <iostream>
#include <glm-src/glm/gtc/matrix_transform.hpp>

namespace FF {
Renderer::Renderer() {
  shaders.emplace("test_shader", std::make_unique<Shader>("res/test.vert", "res/test.frag"));
  shaders.emplace("shape_renderer_shader", std::make_unique<Shader>("res/shape_renderer.vert", "res/shape_renderer.frag"));
}

Renderer::~Renderer() {
}

void Renderer::ClearColor(int r, int g, int b) {
  if (fb.expired())
    return;

  fb.lock()->Bind();
  glClearColor(r / 255.f, g / 255.f, b / 255.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  fb.lock()->Unbind();
}

void Renderer::DrawQuad() {
  DrawQuad(glm::mat4(1.0f));
}

void Renderer::DrawQuad(glm::mat4 transform) {
  DrawQuad(transform, glm::vec4(1.0f));
}

void Renderer::DrawQuad(glm::mat4 transform, glm::vec4 color) {  
  if (fb.expired()) {
    FF_LOG_ERROR("Framebuffer null");    
    return;
  }

  FF::Geometry& q = FF::Geometry::Quad();
  fb.lock()->Bind();
  q.Bind();

  FF::Shader* s = shaders.at("shape_renderer_shader").get();
  if (!s) {
    FF_LOG_WARN("Shader null");
    return;
  }
  
  // FF_LOG_INFO("r={}, g={}, b={}, a={}", color.r, color.g, color.b, color.a);
  s->Bind();
  s->SetUniformMat4("model", transform);
  s->SetUniformMat4("view", glm::mat4(1.0f));
  s->SetUniformMat4("proj", projection);
  // s->SetUniformVec4("color", color);
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  
  s->Unbind();
  q.Unbind();
  fb.lock()->Unbind();
}

void Renderer::SetTargetFramebuffer(std::shared_ptr<FF::Framebuffer> _fb) {
  fb = _fb;
}

void Renderer::UpdateProjectionMatrix(int newX, int newY, int newW, int newH) {
  projection = glm::ortho((float)newX, (float)newW, (float)newH, (float)newY, 0.f, 1000.f);
}
}
