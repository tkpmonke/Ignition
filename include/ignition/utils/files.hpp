#pragma once

#include "types/texture.hpp"

#include <string>
#include <memory>
namespace FS {
   struct _Read {
      std::string data;
      unsigned char* texdata;
      int w, h, nr;
      _Read() {};
   };
   enum _Type {
      Shader,
      Texture,
      Text
   };
   _Read Read(std::string path, _Type type, Ignition::Rendering::Texture* tex);
}
