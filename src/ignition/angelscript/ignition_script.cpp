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

      /*
      S(asEngine->RegisterObjectType("IgnitionScriptBase", 0, asOBJ_REF));

      S(asEngine->RegisterObjectBehaviour("IgnitionScriptBase", asBEHAVE_ADDREF, "void f()", asMETHOD(IgnitionScript, AddRef), asCALL_THISCALL));
      S(asEngine->RegisterObjectBehaviour("IgnitionScriptBase", asBEHAVE_RELEASE, "void f()", asMETHOD(IgnitionScript, Release), asCALL_THISCALL));

      S(asEngine->RegisterObjectMethod("IgnitionScriptBase", "void Start()", asMETHOD(IgnitionScript, Start), asCALL_THISCALL));
      S(asEngine->RegisterObjectMethod("IgnitionScriptBase", "void Shutdown()", asMETHOD(IgnitionScript, Shutdown), asCALL_THISCALL));
      
      S(asEngine->RegisterObjectMethod("IgnitionScriptBase", "void Update()", asMETHOD(IgnitionScript, Update), asCALL_THISCALL));
      S(asEngine->RegisterObjectMethod("IgnitionScriptBase", "void LateUpdate()", asMETHOD(IgnitionScript, LateUpdate), asCALL_THISCALL));
      S(asEngine->RegisterObjectMethod("IgnitionScriptBase", "void FixedUpdate()", asMETHOD(IgnitionScript, FixedUpdate), asCALL_THISCALL));
      S(asEngine->RegisterObjectMethod("IgnitionScriptBase", "void LateFixedUpdate()", asMETHOD(IgnitionScript, LateFixedUpdate), asCALL_THISCALL));
      */
   }
}

#undef S
