#include "utils/files.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace FS {
   std::string Read(std::string path) 
   {
      std::ifstream f(path);
      std::stringstream s;
      s << f.rdbuf();
      return s.str();
   }
}
