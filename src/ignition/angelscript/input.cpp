#include "angelscript/angelscript.hpp"
#include "input.hpp"
#include "camera.hpp"

#define S(x) r = x; assert(r >= 0)

using namespace Ignition::IO;
void Shutdown() {
   Ignition::mainCamera->window->Shutdown();
}

void Reboot() {
   Ignition::mainCamera->window->Restart();
}

void GenericGetMousePos(asIScriptGeneric* gen) {
    *reinterpret_cast<Ignition::Vector2*>(gen->GetAddressOfReturnLocation()) = GetMousePosition();  // Return it by value
}


namespace Ignition::Scripting::AngelScript {
   void RegisterInputFunctions() {
      int r;
      S(asEngine->RegisterGlobalFunction("void Poweroff()", asFUNCTION(Shutdown), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("void Reboot()", asFUNCTION(Reboot), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("float GetScrollX()", asFUNCTION(GetScrollX), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("float GetScrollY()", asFUNCTION(GetScrollY), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("bool GetInput(int)", asFUNCTION(GetInput), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("bool GetInputDown(int)", asFUNCTION(GetInputDown), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("bool GetInputUp(int)", asFUNCTION(GetInputUp), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("bool GetMouse(int)", asFUNCTION(GetMouse), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("bool GetMouseDown(int)", asFUNCTION(GetMouseDown), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("bool GetMouseUp(int)", asFUNCTION(GetMouseUp), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("Vector2 GetMousePosition()",
               asFUNCTION(GenericGetMousePos), asCALL_GENERIC));
      S(asEngine->RegisterGlobalFunction("void LockMouse(bool)", asFUNCTION(LockMouse), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("void HideMouse(bool)", asFUNCTION(HideMouse), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("float GetAxisHorizontal()", asFUNCTION(GetAxisHorizontal), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("float GetAxisVertical()", asFUNCTION(GetAxisVertical), asCALL_CDECL));

   }
}
