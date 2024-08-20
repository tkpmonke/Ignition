#include "types/shader.hpp"

#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <unordered_map>
#include <utility>

std::unordered_map<std::string, int> shaders;

namespace Ignition::Rendering {
   Shader::Shader(std::string vert, std::string frag, ShaderType type)
   {
      this->type = type;
      for (auto i : shaders)
      {
         if (vert+frag == i.first)
         {
            this->program = i.second;
            return;
         }
      }
      

      const char* vShaderCode = vert.c_str();
      const char* fShaderCode = frag.c_str();
      
      unsigned int vertex, fragment;
      
      int success;
      char infoLog[512];
      
      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderCode, NULL);
      glCompileShader(vertex);
      
      glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
      if(!success)
      {
         glGetShaderInfoLog(vertex, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
      }
      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderCode, NULL);
      glCompileShader(fragment);
      
      glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
      if(!success)
      {
         glGetShaderInfoLog(fragment, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
      }  

      this->program = glCreateProgram();
      glAttachShader(this->program, vertex);
      glAttachShader(this->program, fragment);
      glLinkProgram(this->program);

      glGetProgramiv(this->program, GL_LINK_STATUS, &success);
      if (!success)
      {
         glGetProgramInfoLog(this->program, 512, NULL, infoLog);
         std::cout << "error in shader \n" << infoLog << std::endl;
      }

      
      glDeleteShader(vertex);
      glDeleteShader(fragment);

      shaders[vert+frag] = this->program;
   }

   Shader::Shader(std::string data, ShaderType type)
   {
      this->type = type;
      for (auto i : shaders)
      {
         if (vert+frag == i.first)
         {
            this->program = i.second;
            return;
         }
      }
      
      const char* shaderCode = data.c_str();
      int success;
      char infoLog[512];
      uint shader;

      assert(type == ShaderType::Compute);
      // should always be true
      if (type == ShaderType::Compute)
      {
         shader = glCreateShader(GL_COMPUTE_SHADER);
         glShaderSource(shader, 1, &shaderCode, NULL);
         glCompileShader(shader);
         glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
         if(!success)
         {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << "\n";
         }

         this->program = glCreateProgram();
         glAttachShader(this->program, shader);
         glLinkProgram(this->program);

         glDeleteShader(shader);

      }
   }

   void Shader::SetFloat(float v, std::string name)
   {
      glUniform1f(glGetUniformLocation(this->program, name.data()), v);
   }

   void Shader::SetInt(int v, std::string name)
   {
      glUniform1i(glGetUniformLocation(this->program, name.data()), v);
   }

   void Shader::SetBool(bool v, std::string name)
   {
      glUniform1i(glGetUniformLocation(this->program, name.data()), v);
   }

   void Shader::SetDouble(double v, std::string name)
   {
      glUniform1d(glGetUniformLocation(this->program, name.data()), v);
   }

   void Shader::SetMatrix3(Matrix3 v, std::string name)
   {
      glUniformMatrix3fv(glGetUniformLocation(this->program, name.data()), 1, GL_FALSE, glm::value_ptr(v));
   }

   void Shader::SetMatrix4(Matrix4 v, std::string name)
   {
      glUniformMatrix4fv(glGetUniformLocation(this->program, name.data()), 1, GL_FALSE, glm::value_ptr(v));
   }

   void Shader::SetVec2(Vector2 v, std::string name)
   {
      glUniform2f(glGetUniformLocation(this->program, name.data()), v.x, v.y);
   }

   void Shader::SetVec3(Vector3 v, std::string name)
   {
      glUniform3f(glGetUniformLocation(this->program, name.data()), v.x, v.y, v.z);
   }

   void Shader::SetVec4(Vector4 v, std::string name)
   {
      glUniform4f(glGetUniformLocation(this->program, name.data()), v.x, v.y, v.z, v.w);
   }
}
