#include "../include/Shader.hpp"
#include "../include/Logger.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm-src/glm/gtc/type_ptr.hpp>    // glm::value_ptr

namespace FF {
std::string ReadFile(std::ifstream& f) {
  if (!f.is_open())
    throw std::string("Failed to open file");
  std::stringstream ss;
  while (f >> ss.rdbuf());
  return ss.str();
}

Shader::Shader(const std::string& vert_path, const std::string& frag_path)
:vertexShaderPath(vert_path), fragmentShaderPath(frag_path),
vertexShaderHandle(-1), fragmentShaderHandle(-1)
{
  std::ifstream f(vert_path);
  try {
    std::string vert_contents = ReadFile(f);
    vertexShaderHandle   = CompileShader(GL_VERTEX_SHADER, vert_contents);
  } catch (std::string& s) {
    std::cerr << s << std::endl;
  }
  f.close();
  
  std::ifstream f2(frag_path);
  try {
    std::string frag_contents = ReadFile(f2);
    fragmentShaderHandle = CompileShader(GL_FRAGMENT_SHADER, frag_contents);
  } catch (std::string& s) {
    std::cerr << s << std::endl;
  }
  f2.close();
  
  if (vertexShaderHandle == -1 || fragmentShaderHandle == -1) {
    FF_LOG_WARN("One of the shaders failed to compile");
  }
  else {
    shaderProgramHandle = LinkProgram();
    if (shaderProgramHandle == -1) {
      FF_LOG_WARN("The shader program failed to link");
    }
  }
  
  FF_LOG_INFO("Generated shader handle (programId = {})", shaderProgramHandle);
}

Shader::~Shader() {
  Unbind();
  glDeleteProgram(shaderProgramHandle);
  FF_LOG_INFO("Deleted shader handle (programId = {})", shaderProgramHandle);
}

void Shader::Bind() {
  glUseProgram(shaderProgramHandle);
}

void Shader::Unbind() {
  glUseProgram(0);
}

void Shader::SetUniformMat4(const std::string& uniName, glm::mat4 matrix) {
  unsigned int loc = GetUniformLocation(uniName);
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniformVec4(const std::string& uniName, glm::vec4 color) {
  unsigned int loc = GetUniformLocation(uniName);
  glUniform4fv(loc, 4, glm::value_ptr(color));
}

std::string Shader::ShaderTypeToString(GLenum t) {
  switch (t) {
  case GL_VERTEX_SHADER:   return "Vertex";
  case GL_FRAGMENT_SHADER: return "Fragment";
  }
  return "Not a shader type";
}

unsigned int Shader::CompileShader(GLenum type, const std::string& contents) {
  if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
    FF_LOG_ERROR("Type not supported");
    return -1;
  }
  
  unsigned int shader_id = glCreateShader(type);
  const char* source = contents.c_str();
  glShaderSource(shader_id, 1, &source, NULL);
  glCompileShader(shader_id);
  
  int success;
  char infoLog[512];
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
    FF_LOG_ERROR("{} : Compilation failed", ShaderTypeToString(type));
    return -1;
  }
  else {
    FF_LOG_INFO("{} : Compilation successful", ShaderTypeToString(type));
  }
  
  return shader_id;
}

unsigned int Shader::LinkProgram() {
  unsigned int program_id = glCreateProgram();
  glAttachShader(program_id, vertexShaderHandle);
  glAttachShader(program_id, fragmentShaderHandle);
  glLinkProgram(program_id);
  
  int success;
  char infoLog[512];
  glGetShaderiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(program_id, 512, NULL, infoLog);
    FF_LOG_ERROR("Linking failed");
    return -1;
  }
  else {
    FF_LOG_INFO("Linking successful");
  }

  glDeleteShader(vertexShaderHandle);
  glDeleteShader(fragmentShaderHandle);
  return program_id;
}

unsigned int Shader::GetUniformLocation(const std::string& uniformName) {
  // TODO: Cache this
  unsigned int loc = glGetUniformLocation(shaderProgramHandle, uniformName.c_str());
  if (loc == -1) {
    FF_LOG_WARN("Uniform location: {} does not exist", uniformName);
  }
  return loc;
}
}