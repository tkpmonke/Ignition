#include "angelscript/angelscript.hpp"
#include <angelscript/addons/scriptstdstring/scriptstdstring.h>

#include "utils/io.hpp"

namespace Ignition::Scripting::AngelScript {
   asIScriptEngine* asEngine = nullptr;

   void MessageCallback(const asSMessageInfo *msg, void *param) {
      if (msg->type == asMSGTYPE_WARNING ) {
         IO::Warning(std::to_string(msg->row) + (std::string)":" + std::to_string(msg->col) + (std::string)" " + msg->message);
      } else if (msg->type == asMSGTYPE_INFORMATION ) {
         IO::Print(std::to_string(msg->row) + (std::string)":" + std::to_string(msg->col) + (std::string)" " + msg->message);
      } else if (msg->type == asMSGTYPE_ERROR) {
         IO::Error(std::to_string(msg->row) + (std::string)":" + std::to_string(msg->col) + (std::string)" " + msg->message);
      }
   }
   
   void InitilizeAngelScript() {
      asEngine = asCreateScriptEngine();
      asEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
      RegisterAll();

      const char* script = R"(
         
         class Test : IgnitionScript {
            void Start() override {
               Print("Start Called");
            }
         }

         void main() {
            Test t;
            t.Start();

            Print("22");
            Vector2 v;
            v.x = 10;
            v.y = 20;
            Print(v.x);
            Print(v.y);
         }
      )";

      asIScriptModule* mod = asEngine->GetModule("Ignition_Scripts", asGM_ALWAYS_CREATE);
      mod->AddScriptSection("script", script);
      mod->Build();


      asIScriptContext* ctx = asEngine->CreateContext();
      asIScriptFunction* func = mod->GetFunctionByDecl("void main()");

      ctx->Prepare(func);
      ctx->Execute();

      ctx->Release();
      asEngine->ShutDownAndRelease();
   }

   void RegisterAll() {
      RegisterStdString(asEngine);

      RegisterVectors();
      RegisterTransform();
      RegisterIgnitionScript();

      /*
      RegisterModel();
      RegisterShader();
      RegisterMeshRenderer();

      RegisterCollider();
      RegisterRigidbody();

      RegisterObject();
      RegisterCamera();

      RegisterInputKeys();
      RegisterInputFunctions();

      */
      RegisterMiscFunctions();
      RegisterExpose();
   }
}
