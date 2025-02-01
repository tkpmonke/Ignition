#include "angelscript/angelscript.hpp"

#include "modules/physics/rigidbody.hpp"
#include "modules/physics/collider.hpp"

#define S(x) r = x; assert(r >= 0)

using namespace Ignition::Physics;

namespace Ignition::Scripting::AngelScript {
   void RegisterCollider() {
      int r;
      //S(asEngine->RegisterObjectType("Collider", sizeof(Collider), asOBJ_VALUE | asGetTypeTraits<Collider>()));
   }

   void RegisterRigidbody() {
      int r;

      S(asEngine->RegisterObjectType("Rigidbody", sizeof(Rigidbody), asOBJ_REF | asOBJ_NOCOUNT));

      S(asEngine->RegisterObjectProperty("Rigidbody", "Transform@ transform", asOFFSET(Rigidbody, transform)));
      S(asEngine->RegisterObjectProperty("Rigidbody", "Object@ object", asOFFSET(Rigidbody, object)));
      S(asEngine->RegisterObjectProperty("Rigidbody", "bool enabled", asOFFSET(Rigidbody, enabled)));
      S(asEngine->RegisterObjectProperty("Rigidbody", "Vector3 velocity", asOFFSET(Rigidbody, velocity)));      

      S(asEngine->RegisterObjectMethod("Module", "Rigidbody@ opCast()", asFUNCTION((refCast<Ignition::Module, Rigidbody>)), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectMethod("Rigidbody", "Module@ opImplCast()", asFUNCTION((refCast<Rigidbody, Ignition::Module>)), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectMethod("Module", "const Rigidbody@ opCast() const", asFUNCTION((refCast<Ignition::Module, Rigidbody>)), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectMethod("Rigidbody", "const Module@ opImplCast() const", asFUNCTION((refCast<Rigidbody, Ignition::Module>)), asCALL_CDECL_OBJLAST));

   }
}

#undef S
