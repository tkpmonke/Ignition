#include "module.hpp"
#include "types/texture.hpp"

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

      LightType type = Point;

      float distance=5;
      
      const float constant = 1.0f;
      const float linear = 0.09f;
      const float quadratic = 0.032f;

      Vector3 ambient = Vector3(0.05f, 0.05f, 0.05f);
      Vector3 diffuse = Vector3(1,1,1);
      Vector3 specular = Vector3(0.5f, 0.5f, 0.5f);

      // only in use if using spot light
      float cutOff;
      float outerCutOff;

      void Serialize() override;
      void Deserialize() override;
   };

   class DirectionalLight {
   public:
      Vector3 direction = Vector3(-0.2f, -0.6f, -0.3f);
      Vector3 ambient = Vector3(0.05f, 0.05f, 0.05f);
      Vector3 diffuse = Vector3(0.2f, 0.2f, 0.2f);
      Vector3 specular = Vector3(0.5f, 0.5f, 0.5f);

      void Init() { shadowMap = RenderTexture(1024,1024,IGNITION_DEPTH); }

      DirectionalLight() = default;

      RenderTexture shadowMap;
   };
   
   extern std::vector<std::shared_ptr<Light>> lights;
   extern DirectionalLight directionalLight;
   void RenderShadowMaps();
}
