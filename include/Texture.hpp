#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>
#include <glad-build/include/glad/glad.h>

namespace FF {
class Texture {
public:
  Texture(const std::string&);
  ~Texture();
  void Bind();
  void Unbind();
  unsigned int Handle() const;
  
private:
  int width, height, channels;
  unsigned int tex_handle;
};
}

#endif