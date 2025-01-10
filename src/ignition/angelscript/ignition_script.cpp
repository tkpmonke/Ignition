#include "angelscript/angelscript.hpp"
#include "object.hpp"

#define S(x) r = x; assert(r >= 0)

namespace Ignition::Scripting::AngelScript { 
   void RegisterIgnitionScript() {
      int r;
      
      S(asEngine->RegisterInterface("IgnitionScript"));

      S(asEngine->RegisterInterfaceMethod("IgnitionScript", "void Start()"));
      S(asEngine->RegisterInterfaceMethod("IgnitionScript", "void Shutdown()"));
      
      S(asEngine->RegisterInterfaceMethod("IgnitionScript", "void Update()"));
      S(asEngine->RegisterInterfaceMethod("IgnitionScript", "void LateUpdate()"));
      S(asEngine->RegisterInterfaceMethod("IgnitionScript", "void FixedUpdate()"));
      S(asEngine->RegisterInterfaceMethod("IgnitionScript", "void LateFixedUpdate()"));
   }
}

#undef S
