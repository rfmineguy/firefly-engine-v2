#include "../include/Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace FF {
std::string ReadFile(std::ifstream& f) {
  if (!f.is_open())
    throw std::string("Failed to open file");
  std::stringstream ss;
  while (f >> ss.rdbuf());
  return ss.str();
}

Shader::Shader(const std::string& vert_path, const std::string& frag_path)
:vertexShaderPath(vert_path), fragmentShaderPath(frag_path)
{
  std::cout << "Reading file" << std::endl;
  std::ifstream f(vert_path);
  std::string vert_contents = ReadFile(f);
  f.close();
  
  std::ifstream f2(frag_path);
  std::string frag_contents = ReadFile(f2);
  f2.close();
  
  std::cout << vert_contents << std::endl;
  std::cout << frag_contents << std::endl;
    
  vertexShaderHandle   = CompileShader(GL_VERTEX_SHADER, vert_contents);
  fragmentShaderHandle = CompileShader(GL_FRAGMENT_SHADER, frag_contents);
  if (vertexShaderHandle == -1 || fragmentShaderHandle == -1) {
    std::cerr << "One of the shaders failed to compile" << std::endl; 
  }
  else {
    shaderProgramHandle = LinkProgram();
    if (shaderProgramHandle == -1) {
      std::cerr << "The shader program failed to link" << std::endl; 
    }
  }
  
  // The shader is valid here
}

Shader::~Shader() {
  Unbind();
  glDeleteProgram(shaderProgramHandle);
}

void Shader::Bind() {
  glUseProgram(shaderProgramHandle);
}

void Shader::Unbind() {
  glUseProgram(0);
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
    std::cerr << "type not supported" << std::endl;
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
    std::cerr << "Error: " << ShaderTypeToString(type) << " : Compilation failed" << std::endl;
    return -1;
  }
  else {
    std::cout << "Info: " << ShaderTypeToString(type) << " : Compilation successful" << std::endl;
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
    std::cerr << "Error: Shader linking failed" << std::endl;
    return -1;
  }
  else {
    std::cerr << "Info: Shader linking successful" << std::endl;
  }

  glDeleteShader(vertexShaderHandle);
  glDeleteShader(fragmentShaderHandle);
  return program_id;
}
}