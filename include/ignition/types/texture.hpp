#pragma once

#include "GL/glew.h"
#include <string>
#include <unordered_map>
#include <vector>

#define IGNITION_CUBE_MAP     4
#define IGNITION_2D           2

namespace Ignition::Rendering {

   extern std::unordered_map<std::string, int> texture_lookup_table;

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

      void SetFlags(int flags) { this->flags = flags; }
      void SetType(int type) { this->type = type; }
      void LoadData(std::string file);
      void LoadData(std::vector<std::string> file);
      void LoadData(std::vector<const char*> data, int w, int h, int nr, std::string name);
      void LoadData(unsigned char* data, int w, int h, int nr, std::string name);
      unsigned int location = 0;
      std::string name;
      operator int() const { return location; };
      int type = 2;
   private:
      int flags;
      void LoadData(unsigned char* data, int w, int h, int nr, std::string name, int i);
   };
}
