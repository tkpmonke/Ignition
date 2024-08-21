#include "postprocess.hpp"
#include "window.hpp"

#include <vector>

namespace Ignition::Rendering {
   class PPManager {
   public:
      struct PPEffect {
         std::string name;
         bool enabled;
         PostProcess pp;
      };

      std::vector<PPEffect> effects;
      std::vector<int>      index;

      PPManager(Window*);
      PPManager() = default;
      void RenderEffects();

   private:
      Window* window;
   };

   extern PPManager pp_manager;
}
