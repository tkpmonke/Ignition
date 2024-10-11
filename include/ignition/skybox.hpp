#include "types/texture.hpp"
#include "object.hpp"

namespace Ignition {
   class Skybox {
   public:
      Rendering::Texture tex;

      Skybox(std::vector<std::string> textures);
      Skybox(std::vector<const char*> textures, int w, int h, int nr, std::string name);
      Skybox() = default; 
      void Render();

   private:
      Object o;
   };
}
