#pragma once

#include "types/texture.hpp"
#include "module_registry.hpp"

#include <memory>
#include <vector>

namespace Ignition::Rendering {
   enum LightType {
      Spot = 0,
      Point = 1
   };

   class Light : public Module {
   public:
      IGMODULE(Light);

      LightType type = Point;

      float power = 1;
      float fallOff = 0.009f;
      
      const float constant = 1.0f;
      const float linear = 0.09f;
      const float quadratic = 0.032f;

      Vector3 ambient = Vector3(0.05f, 0.05f, 0.05f);
      Vector3 diffuse = Vector3(1,1,1);
      Vector3 specular = Vector3(0.5f, 0.5f, 0.5f);

      // only in use if using spot light
      float cutOff = 25;
      float outerCutOff = 30;

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
