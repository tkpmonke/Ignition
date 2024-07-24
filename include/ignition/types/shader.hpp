#pragma once

#include <GL/glew.h>

#include <string>

namespace Ignition::Rendering {
   class Shader {
   
      void Load(std::string vert, std::string frag);

      int program;
   };
}
