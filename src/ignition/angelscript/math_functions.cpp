#include "angelscript/angelscript.hpp"
#include "angelscript/addons/scriptmath/scriptmath.h"
#include "angelscript/addons/scriptmath/scriptmathcomplex.h"

#include <assert.h>
#include <cmath>

#define S(x) r = x; assert(r >= 0);

namespace Ignition::Scripting::AngelScript {
   void RegisterMathFunctions() {
      RegisterScriptMath(asEngine);
      RegisterScriptMathComplex(asEngine);
   }
}

