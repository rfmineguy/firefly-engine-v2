#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

namespace FF {
class Framebuffer {
public:
  Framebuffer() = delete;
  Framebuffer(int, int);
  ~Framebuffer();
  
  void Resize(int, int);
  void Bind();
  void Unbind();
  unsigned int GetColorAttachment() const;
  
private:
  unsigned int width, height;
  unsigned int fbo, rbo;
  unsigned int color_attachment_tex;
};
}

#endif