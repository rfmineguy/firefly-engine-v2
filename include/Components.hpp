#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <string>
#include <glm-src/glm/glm.hpp>

struct Identifier {
  Identifier(const std::string& id): id(id) {}
  Identifier(const char* id): id(std::string(id)) {}
  std::string id;
};

struct Transform {
  Transform() {}
  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  glm::vec3 rotation_center;
};

struct ShapeRenderer {
  enum Shape {
    RECTANGLE, TRIANGLE, CIRCLE, POLYGON
  };
  ShapeRenderer() {}
  glm::vec4 color;
  Shape shape;
};

struct SpriteRenderer {
  SpriteRenderer() {}
  glm::vec4 color_tint;
};

#endif