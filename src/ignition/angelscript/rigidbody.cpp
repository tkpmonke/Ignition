#include "angelscript/angelscript.hpp"

#include "modules/physics/rigidbody.hpp"
#include "modules/physics/collider.hpp"

#define S(x) r = x; assert(r >= 0)

namespace Ignition::Scripting::AngelScript {
   void RegisterCollider() {
      int r;
      S(asEngine->RegisterObjectType("Transform", sizeof(Ignition::Transform), asOBJ_REF | asOBJ_NOCOUNT));
   }

   void RegisterRigidbody() {
      int r;
   }
}

#undef S
