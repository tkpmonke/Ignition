#pragma once

#include <angelscript/include/angelscript.h>

#include <string>
#include <unordered_set>
namespace Ignition::Scripting::AngelScript {
   extern asIScriptEngine* asEngine;
   extern asIScriptModule* igModule;

   template<class A, class B>
   B* refCast(A* a) {
       if (a == nullptr) return nullptr;
       return (B*)a;
   }

   void InitilizeAngelScript();
   std::string PreprocessScript(const std::string& filePath, std::unordered_set<std::string>& includedFiles); 
   void RegisterAll();

   void Shutdown();

   // general
   void RegisterIgnitionScript();
   void RegisterVectors();
   void RegisterTransform();
   void RegisterObject();
   void RegisterCamera();
   void RegisterModule();

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
   void RegisterMathFunctions();
}
