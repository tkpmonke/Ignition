#include "module.hpp"

namespace Ignition::Rendering {
   enum LightType {
      Spot,
      Point,
      Directional
   };

   class Light : public Module {
      CREATE_MODULE("Light");

      LightType type;
      

   };
}
