#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <glm-src/glm/glm.hpp>
#include <vector>

namespace FF {
class Geometry {
public:
  struct Vertex {
    glm::vec3 position;
  };

public:
  // Usage:
  //   FF::Geometry& g = FF::Geometry::Quad();
  static Geometry& Quad() {
    static Geometry g;
    
    // We already generated everything for the quad
    if (g.has_generated_buffers)
      return g;

    g.vertices.push_back(Vertex{ .position=glm::vec3( 0.5f,  0.5f, 0.0f) });
    g.vertices.push_back(Vertex{ .position=glm::vec3( 0.5f, -0.5f, 0.0f) });
    g.vertices.push_back(Vertex{ .position=glm::vec3(-0.5f, -0.5f, 0.0f) });
    g.vertices.push_back(Vertex{ .position=glm::vec3(-0.5f,  0.5f, 0.0f) });
    g.indices.assign({0, 1, 3, 1, 2, 3});
    g.GenerateGLHandles();
    
    g.has_generated_buffers = true;
    return g;
  }

private:
  Geometry();
  ~Geometry();
  
private:
  void GenerateGLHandles();

public:
  void Bind();
  void Unbind();
  
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

private:
  bool has_generated_buffers;
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;

friend class Renderer;
};
}

#endif