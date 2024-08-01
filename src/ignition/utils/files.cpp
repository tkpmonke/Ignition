#include "utils/files.hpp"

#include <iostream>
#include <fstream>
#include <sstream>


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
      }
      return r;
   }
}
