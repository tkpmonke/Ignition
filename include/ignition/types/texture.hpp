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
      void LoadData(std::string file, std::string name);
      void LoadData(unsigned char* data, int w, int h, int nr, std::string name);
      unsigned int location = 0;
      operator int() const { return location; };
   private:
      int flags;
   };
}
