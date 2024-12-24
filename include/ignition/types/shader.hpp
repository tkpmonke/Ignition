#pragma once

#include <GL/glew.h>
#include "types/transform.hpp"
#include "types/texture.hpp"

#include <string>
#include <unordered_map>

namespace Ignition::Rendering {
   extern std::unordered_map<std::string, int> shader_lookup_table;
   
   enum ShaderType {
      Unlit = 0,
      Lit = 1,
      Compute = 2,
   }; 
   class Shader {
   public:   
      // use this for most shaders
      Shader(std::string vert, std::string frag, ShaderType type);
      Shader(std::string vert, std::string frag, int type);
      // use this for compute shaders
      Shader(std::string data, ShaderType type);
      Shader(std::string data, int type);

      Shader() = default;

      std::string vert, frag;

      unsigned int program;
      
      Vector4 color = Vector4(1,1,1,1);
      Texture albedo;
      Texture diffuse;
      float shininess;
      float intensity = 1.f;
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
