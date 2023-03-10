#ifndef SHADER_HPP
#define SHADER_HPP
#include <string>
#include <glad-build/include/glad/glad.h>
#include <glm-src/glm/glm.hpp>

namespace FF {
class Shader {
private:
  std::string ShaderTypeToString(GLenum t);
  
public:
  Shader(const std::string&, const std::string&);
  ~Shader();
  void Bind();
  void Unbind();
  
public:
  void SetUniformMat4(const std::string&, glm::mat4);
  
private:
  unsigned int CompileShader(GLenum, const std::string&);
  unsigned int LinkProgram();
  
private:
  unsigned int GetUniformLocation(const std::string&);

private:
  std::string vertexShaderPath;
  std::string fragmentShaderPath;
  unsigned int vertexShaderHandle;
  unsigned int fragmentShaderHandle;
  unsigned int shaderProgramHandle;
};
}

#endif