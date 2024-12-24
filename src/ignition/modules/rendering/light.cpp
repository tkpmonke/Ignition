#include "modules/rendering/light.hpp"
#include "utils/files.hpp"

using namespace Ignition::IO;
namespace Ignition::Rendering {
   std::vector<std::shared_ptr<Light>> lights;

   void Light::Serialize() {
      Write8(type);
      WriteFloat(distance);
      WriteVector3(ambient);
      WriteVector3(diffuse);
      WriteVector3(specular);

      if (type == Spot) {
         WriteFloat(cutOff);
         WriteFloat(outerCutOff);
      }
   }

   void Light::Deserialize() {
      type = (LightType)Read8();
      distance = ReadFloat();

      ambient = ReadVector3();
      diffuse = ReadVector3();
      specular = ReadVector3();

      if (type == Spot) {
         cutOff = ReadFloat();
         outerCutOff = ReadFloat();
      }
   }
}
