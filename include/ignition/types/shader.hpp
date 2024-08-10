#pragma once

#include <GL/glew.h>
#include "types/transform.hpp"
#include "types/texture.hpp"

#include <string>

namespace Ignition::Rendering {
   enum ShaderType {
      Unlit = 0,
      Lit = 1,
   }; 
   class Shader {
   public:   
      Shader(std::string vert, std::string frag, ShaderType type);
      Shader() = default;

      std::string vert, frag;

      unsigned int program;
      
      Vector4 color = Vector4(1,1,1,1);
      Texture albedo;
      Texture diffuse;
      float shininess;
      Vector3 specular;

      void SetFloat(float v, std::string name);
      void SetInt(int v, std::string name);
      void SetBool(bool v, std::string name);
      void SetDouble(double v, std::string name);
      void SetMatrix4(Matrix4 v, std::string name);
      void SetMatrix3(Matrix3 v, std::string name);
      void SetVec2(Vector2 v, std::string name);
      void SetVec3(Vector3 v, std::string name);
      void SetVec4(Vector4 v, std::string name);

      ShaderType type;
   };
}
