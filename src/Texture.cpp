#include "../include/Texture.hpp"
#include "../include/Logger.hpp"
#include <stb-src/stb_image.h>

namespace FF {
Texture::Texture(const std::string& path) {
  // generate textures
  glGenTextures(1, &tex_handle);
  glBindTexture(GL_TEXTURE_2D, tex_handle);    
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  stbi_set_flip_vertically_on_load(1);
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (data != nullptr) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    FF_LOG_ERROR("Failed to load texture data");
  }
  stbi_image_free(data);
  FF_LOG_INFO("Created texture {} {}", path, tex_handle);
}

Texture::~Texture() {
  glDeleteTextures(1, &tex_handle);
  FF_LOG_INFO("Deleted texture {}", tex_handle);
}

void Texture::Bind() {
  glBindTexture(GL_TEXTURE_2D, tex_handle);
}

void Texture::Unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::Handle() const {
  return tex_handle;
}

int Texture::GetWidth() const {
  return width;
}

int Texture::GetHeight() const {
  return height;
}
}