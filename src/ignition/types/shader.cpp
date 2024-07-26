#include "types/shader.hpp"

namespace Ignition::Rendering {
   Shader::Shader(std::string vert, std::string frag)
   {
      const char* vShaderCode = vert.c_str();
      const char* fShaderCode = frag.c_str();
      
      unsigned int vertex, fragment;
      
      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderCode, NULL);
      glCompileShader(vertex);
        
      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderCode, NULL);
      glCompileShader(fragment);
        
      this->program = glCreateProgram();
      glAttachShader(this->program, vertex);
      glAttachShader(this->program, fragment);
      glLinkProgram(this->program);
        
      glDeleteShader(vertex);
      glDeleteShader(fragment);
   }
}
