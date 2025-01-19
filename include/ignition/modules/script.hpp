#pragma once

#include "module.hpp"
//#include "lua/lualib.hpp"
//#include <vector>

#include <angelscript/angelscript.hpp>

//using namespace Ignition::Scripting::Lua;
using namespace Ignition::Scripting::AngelScript;

namespace Ignition {

   class asFunction {
   public:
      asIScriptFunction* ptr = nullptr;
      operator asIScriptFunction*() { return ptr; }

      asFunction() : ptr(nullptr) {}
      asFunction(asIScriptFunction* ptr) : ptr(ptr) {}

      void operator()(asIScriptContext* ctx) {
         ctx->Prepare(ptr);
         ctx->Execute();
      }

      void operator()(asIScriptContext* ctx, asIScriptObject* obj) {
         ctx->Prepare(ptr);
         ctx->SetObject(obj);
         ctx->Execute();
      }
   };

   class Script : public Module {
   public:
      CREATE_MODULE("Script")

      // defaults to lua
      enum { 
         AngelScript = 0,
      } language = AngelScript;

      std::string path;
      
      void Start() override;
      void Update() override;
      void Shutdown() override;
      void Serialize() override;
      void Deserialize() override;

   private:
      asIScriptContext* ctx;

      asIScriptModule* module;
      asIScriptObject* instance;

      asFunction start;
      asFunction shutdown;
      asFunction update;
      asFunction lateUpdate;
      asFunction fixedUpdate;
      asFunction lateFixedUpdate;

      void RegisterGlobalVariables();
   };
}
