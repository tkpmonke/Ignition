#include "angelscript/angelscript.hpp"
#include <assert.h>

namespace Ignition::Scripting::AngelScript {
   void RegisterExpose() {
      int r = asEngine->RegisterInterface("Expose");
      assert(r >= 0);
   }
}
