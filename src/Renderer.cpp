#include "../include/Renderer.hpp"
#include "../include/Logger.hpp"
#include <glad/glad.h>
#include <iostream>
#include <glm-src/glm/gtc/matrix_transform.hpp>

namespace FF {
Renderer::Renderer() {
  shaders.emplace("test_shader", std::make_unique<Shader>("res/shaders/test.vert", "res/shaders/test.frag"));
  shaders.emplace("shape_renderer_shader", std::make_unique<Shader>("res/shaders/shape_renderer.vert", "res/shaders/shape_renderer.frag"));
}

Renderer::~Renderer() {
}

void Renderer::ClearColor(float r, float g, float b, float a, std::shared_ptr<Framebuffer> target) {
  target->Bind();
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
  target->Unbind();
}

void Renderer::DrawLine(glm::vec3 p1, glm::vec3 p2, float thickness, std::shared_ptr<Framebuffer> target) {
  target->Bind();
  
  target->Bind();
}

void Renderer::DrawQuad(glm::mat4 model, glm::mat4 view, glm::mat4 projection, std::shared_ptr<FF::Framebuffer> target) {
  target->Bind();
  FF::Shader* p_shader = shaders.at("shape_renderer_shader").get();
  FF::Geometry::Quad().Bind();
  p_shader->Bind();
  p_shader->SetUniformMat4("model", model);
  p_shader->SetUniformMat4("view", view);
  p_shader->SetUniformMat4("proj", projection);
  // p_shader->SetUniformVec4("u_color", glm::vec4(1.f));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  p_shader->Unbind();
  FF::Geometry::Quad().Unbind();
  target->Unbind();
}
}
