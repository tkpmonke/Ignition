#include "utils/model_loader.hpp"
#include "utils/files.hpp"
#include "utils/str.hpp"

#include <iostream>
#include <fstream>

namespace Ignition::ModelLoader {
   Model LoadOBJ(std::string path)
   {
      Model m;
      std::string line;
      std::ifstream s(path);

      while (std::getline(s, line))
      {
         std::vector<std::string> spl = STR::split(&line, ' '); 
         if (spl[0] == "v")
         {

         }
      }
      

      return m;
   }
}
