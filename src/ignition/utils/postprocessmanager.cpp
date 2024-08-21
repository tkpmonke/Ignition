#include "utils/postprocessingmanager.hpp"
#include "utils/postprocessing_shaders.hpp"

namespace Ignition::Rendering {
   PPManager pp_manager;

   PPManager::PPManager(Window* window) : window(window) {
      effects.push_back({"Invert", false, PostProcess(pp_invert, this->window)});
      effects.push_back({"Vignette", false, PostProcess(pp_vignette, this->window)});

      for (int i = 0; i < effects.size(); ++i)
      {
         index.push_back(i);
      }
   }

   void PPManager::RenderEffects() {
      for (int i : index) {
         if (effects[i].enabled) effects[i].pp.Render();
      }
   }
}
