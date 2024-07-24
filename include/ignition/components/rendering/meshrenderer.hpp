#pragma once

#include "types/shader.hpp"

#include <GL/glew.h>
#include <module.hpp>

namespace Ignition::Rendering {
   class MeshRenderer : public Module{
   public:
      CREATE_MODULE("Mesh Renderer");

      Shader shader;
      
      MeshRenderer() = default;
      
      void LoadShader(Shader s) {this->shader = s;}

   private:
      uint vao;
   };

}
