#include "types/texture.hpp"
#include "object.hpp"

namespace Ignition {
   class Skybox {
   public:
      Rendering::Texture tex;

      Skybox(std::vector<std::string> textures);
      Skybox() = default; 
      void Render();

   private:
      Object o;
   };
}
