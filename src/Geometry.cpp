#include "../include/Geometry.hpp"
#include "../include/Logger.hpp"
#include <iostream>
#include <glad-build/include/glad/glad.h>

namespace FF {
Geometry::Geometry() {}

Geometry::~Geometry() {
  glDeleteBuffers(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  FF_LOG_INFO("Deleted geometry handles (vao = {})", vao);
}

void Geometry::GenerateGLHandles() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
  glEnableVertexAttribArray(0);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
  FF_LOG_INFO("Generated geometry handles (vao = {})", vao);
}

void Geometry::Bind() {
  glBindVertexArray(vao);
}

void Geometry::Unbind() {
  glBindVertexArray(0);
}
}