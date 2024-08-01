#pragma once

#include "GL/glew.h"
#include <string>

namespace Ignition::Rendering {
   enum TextureFlags {
      Repeat,
      Clamp,
      Mirrored_Repeat,
      
      Nearest,
      Linear
   };

   class Texture {
   public:
      Texture() = default;

      void SetFlags(int flags) {this->flags = flags;}
      void LoadData(std::string file);
      unsigned int location = 0;
      unsigned int id;
      operator int() const { return location; };
   private:
      int flags;
   };
}
