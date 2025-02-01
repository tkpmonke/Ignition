#include "angelscript/angelscript.hpp"
#include "angelscript/addons/scriptmath/scriptmath.h"
#include "angelscript/addons/scriptmath/scriptmathcomplex.h"

#include <assert.h>
#include <cmath>

#define S(x) r = x; assert(r >= 0);
float min(float a, float b) {
   return std::min(a, b);
}
float max(float a, float b) {
   return std::max(a, b);
}

namespace Ignition::Scripting::AngelScript {
   void RegisterMathFunctions() {
      RegisterScriptMath(asEngine);
	   int r;
      r = asEngine->RegisterGlobalFunction("float min(float,float)", asFUNCTION(min), asCALL_CDECL); assert( r >= 0 );
	   r = asEngine->RegisterGlobalFunction("float max(float,float)", asFUNCTION(max), asCALL_CDECL); assert( r >= 0 );
   }
}

