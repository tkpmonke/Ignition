#pragma once

#include <angelscript/include/angelscript.h>

namespace Ignition::Scripting::AngelScript {
   extern asIScriptEngine* asEngine;

   void InitilizeAngelScript();
   void RegisterAll();

   // general
   void RegisterIgnitionScript();
   void RegisterVectors();
   void RegisterTransform();
   void RegisterObject();
   void RegisterCamera();

   // rendering
   void RegisterModel();
   void RegisterShader();
   void RegisterMeshRenderer();

   // physics
   void RegisterCollider();
   void RegisterRigidbody();
   
   // input
   void RegisterInputKeys();
   void RegisterInputFunctions();

   // random
   void RegisterMiscFunctions();
   void RegisterExpose();
}
