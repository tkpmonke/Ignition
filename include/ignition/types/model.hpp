#pragma once

#include <vector>
#include <string>

namespace Ignition {
   class Model {
   public:
      std::vector<uint> indices;
      std::vector<float> vertices, normals, uv;

      std::string path, name;
   };
}
