#include "module.hpp"

#include <memory>
#include <vector>

namespace Ignition::Rendering {
   enum LightType {
      Spot = 0,
      Point = 1
   };

   class Light : public Module {
   public:
      CREATE_MODULE("Light");

      LightType type;

      float distance=5;
      
      const float constant = 1.0f;
      const float linear = 0.09f;
      const float quadratic = 0.032f;

      Vector3 ambient;
      Vector3 diffuse;
      Vector3 specular;

      // only in use if using spot light
      float cutOff;
      float outerCutOff;

      void Serialize() override;
      void Deserialize() override;
   };
   
   extern std::vector<std::shared_ptr<Light>> lights;
}
