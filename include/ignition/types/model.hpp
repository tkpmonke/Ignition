#include <vector>

namespace Ignition {
   class Model {
   public:
      std::vector<int> indices;
      std::vector<float> vertices, normals, uv;
   };
}
