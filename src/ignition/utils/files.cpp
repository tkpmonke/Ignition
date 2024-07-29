#include "utils/files.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

namespace FS {
   _Read Read(std::string path, _Type type, Ignition::Rendering::Texture* tex) 
   {
      _Read r = _Read();
      if (type == Shader || type == Text) {
         std::ifstream f(path);
         std::stringstream s;
         s << f.rdbuf();
         r.data = s.str();
      } else if (type == Texture) {
         stbi_set_flip_vertically_on_load(true);
         r.texdata = stbi_load(path.data(), &r.w, &r.h, &r.nr, 0);
         if (!r.texdata)
         {
            std::cerr << "texture can't be loaded\n";
         }
         else
         {
            tex->LoadData(r.texdata, r.w, r.h, r.nr);
         }  
      }
      return r;
   }
}
