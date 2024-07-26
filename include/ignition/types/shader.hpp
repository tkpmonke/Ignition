#pragma once

#include <GL/glew.h>

#include <string>

namespace Ignition::Rendering {
   class Shader {
   public:   
      Shader(std::string vert, std::string frag);
      Shader() = default;

      int program;
   };
}
