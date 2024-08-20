#include "postprocess.hpp"
#include "window.hpp"

#include <vector>

namespace Ignition::Rendering {
   class PPManager {
      struct PPEffect {
         std::string name;
         std::string data;
         PostProcess pp;
      };

      std::vector<int> active_effects;
      std::vector<PPEffect> all_effects;

      PPManager() = default;

      void RenderEffects(Window*);
   };

   extern PPManager pp_manager;
}
