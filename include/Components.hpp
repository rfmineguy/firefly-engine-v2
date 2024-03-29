#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <string>
#include <glm-src/glm/glm.hpp>
#include <glm-src/glm/gtx/transform.hpp>
#include <entt-src/src/entt/entt.hpp>
#include <iostream>
#include <vector>


struct Behavior {
  
};

struct Relationship {
  entt::entity parent;
  std::vector<entt::entity> children;
  bool is_dirty;
};

struct Identifier {
  std::string id;
  Identifier(const std::string& id): id(id) {}
  // Identifier(const char* id): id(std::string(id)) {}

  friend std::ostream& operator<<(std::ostream& os, const Identifier& identifier) {
    os << "Identifier { " << std::endl;
    os << "  id -> " << identifier.id << std::endl;
    os << "}" << std::endl;
    return os;
  }
};

struct Transform {
  Transform()
  :position(glm::vec3(5, 0, 0)), 
  scale(glm::vec3(10)),
  rotation(glm::vec3(0)),
  rotation_center(glm::vec3(0.5)) {}

  friend std::ostream& operator<<(std::ostream& os, const Transform& t) {
    os << "Transform { " << std::endl;
    os << "  position -> x: " << t.position.x << ", y: " << t.position.y << ", z: " << t.position.z << std::endl;
    os << "  rotation -> x: " << t.rotation.x << ", y: " << t.rotation.y << ", z: " << t.rotation.z << std::endl;
    os << "  scale    -> x: " << t.scale.x    << ", y: " << t.scale.y    << ", z: " << t.scale.z << std::endl;
    os << "}" << std::endl;
    return os;
  }

  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  glm::vec3 rotation_center;

  glm::mat4 cumulative_transformation;

  glm::mat4 CalcModelMatrix() {
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::translate(model, rotation_center);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(0, 0, 1));
    model = glm::translate(model, -rotation_center);
    return model;
  }
};

struct ShapeRenderer {
  enum Shape {
    RECTANGLE, TRIANGLE, CIRCLE, POLYGON, SHAPE_COUNT
  };
  ShapeRenderer() {}

  friend std::ostream& operator<<(std::ostream& os, const ShapeRenderer& sr) {
    os << "ShapeRenderer { " << std::endl;
    os << "  color -> r: " << sr.color.r << ", g: " << sr.color.g << ", b: " << sr.color.b << ", a: " << sr.color.a << std::endl;
    os << "  shape -> " << sr.shape << std::endl; 
    os << "}" << std::endl;
    return os;
  }

  glm::vec4 color;
  Shape shape;
};

struct SpriteRenderer {
  SpriteRenderer() {}
  glm::vec4 color_tint;
  friend std::ostream& operator<<(std::ostream& os, const SpriteRenderer& sr) {
    os << "SpriteRenderer { " << std::endl;
    os << "  color_tint -> r: " << sr.color_tint.r << ", g: " << sr.color_tint.g << ", b: " << sr.color_tint.b << ", a: " << sr.color_tint.a << std::endl;
    os << "}" << std::endl;
    return os;
  }
};

#endif