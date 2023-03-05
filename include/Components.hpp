#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <string>

struct Identifier {
  Identifier(const std::string& id): id(id) {}
  std::string id;
};

#endif