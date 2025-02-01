#include "modules/script.hpp"
#include "utils/io.hpp"
#include "utils/files.hpp"
#include "angelscript/angelscript.hpp"

#include "scene.hpp"

#include <filesystem>
#include <vector>

#define CALL(x) if (x.ptr != nullptr) x(ctx, instance);
#define S(x) r = x; assert(r >= 0);

namespace Ignition {
   bool allowScripting = !Ignition::IO::InEditor();
   const char* ignitionScript = R"(
         class IgnitionScriptBase : IgnitionScript {
            void Start() {}
            void Shutdown() {}
            void Update() {}
            void LateUpdate() {}
            void FixedUpdate() {}
            void LateFixedUpdate() {}
         }
   )";

   float time = 0;
   Ignition::Transform t;
   Ignition::Object o;
   void Script::RegisterGlobalVariables() {
      int r;

      S(asEngine->RegisterGlobalProperty("Transform transform", &t));
      S(asEngine->RegisterGlobalProperty("Object@ object", nullptr));
      S(asEngine->RegisterGlobalProperty("float time", &time));
   }

   bool registered = 0;

   void Script::Start() {

      //if (std::filesystem::exists(Ignition::IO::GetProjectHome()+path) && !std::filesystem::is_directory(Ignition::IO::GetProjectHome()+path)) {
         std::unordered_set<std::string> includedFiles;

         std::string s = path;
         s.erase(std::remove(s.begin(), s.end(), '/'), s.end());
         s.erase(std::remove(s.begin(), s.end(), '.'), s.end());
         s += std::to_string(object->id);
         s += std::to_string(id);

         asIScriptModule* module = asEngine->GetModule(s.data(), asGM_ALWAYS_CREATE);
         if (!module) {
            return;
         }

         IO::DebugPrint(Ignition::IO::GetProjectHome()+path);
      
         if (!registered) 
            this->RegisterGlobalVariables();
         registered = true;

         module->AddScriptSection("ignitionScriptBase", ignitionScript);
         module->AddScriptSection("script", PreprocessScript(Ignition::IO::GetProjectHome()+path, includedFiles).data());
         module->Build();

         ctx = asEngine->CreateContext();
         asITypeInfo* interface = module->GetTypeInfoByName("IgnitionScriptBase");
         asITypeInfo* scriptClass = nullptr;
         
         int typeCount = module->GetObjectTypeCount();
         for (int i = 0; i < typeCount; ++i) {
            asITypeInfo* typeInfo = module->GetObjectTypeByIndex(i);
            if (typeInfo != nullptr && (typeInfo->GetFlags() & asOBJ_REF)) {
               if (typeInfo->GetBaseType() == interface) {
                  scriptClass = typeInfo;
               }
            }
         }

         if (scriptClass != nullptr) {
            std::string factory = scriptClass->GetName() + (std::string)" @"
                                + scriptClass->GetName() + (std::string)"()";
            asFunction f(scriptClass->GetFactoryByDecl(factory.data()));
            f(ctx);

            instance = *(asIScriptObject**)ctx->GetAddressOfReturnValue();
            instance->AddRef();

            this->start = asFunction(scriptClass->GetMethodByDecl("void Start()"));
            this->update = asFunction(scriptClass->GetMethodByDecl("void Update()"));
            this->shutdown = asFunction(scriptClass->GetMethodByDecl("void Shutdown()"));
            this->fixedUpdate = asFunction(scriptClass->GetMethodByDecl("void FixedUpdate()"));
            this->lateUpdate = asFunction(scriptClass->GetMethodByDecl("void LateUpdate()"));
            this->lateFixedUpdate = asFunction(scriptClass->GetMethodByDecl("void LateFixedUpdate()"));
         }
         
         t = *this->transform;
         o = *this->object;
         CALL(this->start);
         *this->transform = t;
      //}
   }

   void Script::Update() {
      time = glfwGetTime();
      o = *this->object;
      t = *this->transform;
      CALL(this->update);
      *this->transform = t;
   }

   void Script::Shutdown() {
      CALL(this->shutdown);

      instance->Release();
      ctx->Release();

   }

   void Script::Serialize() {
      Ignition::IO::WriteString(path);
   }

   void Script::Deserialize() {
      this->path = Ignition::IO::ReadString();
   }
}

#undef CALL
#undef S
