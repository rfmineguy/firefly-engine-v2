#include "../include/Framebuffer.hpp"
#include <glad-build/include/glad/glad.h>
#include <iostream>

namespace FF {
Framebuffer::Framebuffer(int w, int h)
:width(w), height(h) {
  /*
    Unfortunately, we can't use our framebuffer yet because it is not complete. For a framebuffer to be complete the following requirements have to be satisfied:

    We have to attach at least one buffer (color, depth or stencil buffer).
    There should be at least one color attachment.
    All attachments should be complete as well (reserved memory).
    Each buffer should have the same number of samples.
  */  
  Resize(w, h);
}

Framebuffer::~Framebuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fbo);
  glDeleteTextures(1, &color_attachment_tex);
  std::cout << "Deleted framebuffer" << std::endl;
}

void Framebuffer::Resize(int newWidth, int newHeight) {
  glDeleteFramebuffers(1, &fbo);
  glDeleteTextures(1, &color_attachment_tex);
  glDeleteRenderbuffers(1, &rbo);
  
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  
  // Create color texture
  glGenTextures(1, &color_attachment_tex);
  glBindTexture(GL_TEXTURE_2D, color_attachment_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_tex, 0);
  
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
    // std::cout << "Framebuffer complete" << std::endl;
  }
  else {
    std::cout << "Framebuffer incomplete" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::Unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::GetColorAttachment() const {
  return color_attachment_tex;
}
}
