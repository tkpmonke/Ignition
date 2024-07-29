#pragma once

#include "GL/glew.h"

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
      void LoadData(unsigned char* data, int w, int h, int nr);
      unsigned int location;
      operator int() const { return location; };
   private:
      int flags;
   };
}
