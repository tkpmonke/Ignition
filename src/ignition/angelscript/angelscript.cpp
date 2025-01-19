#include "angelscript/angelscript.hpp"
#include <angelscript/addons/scriptstdstring/scriptstdstring.h>

#include "utils/io.hpp"

#include <fstream>
#include <sstream>
#include <unordered_set>
#include <assert.h>

namespace Ignition::Scripting::AngelScript {
   asIScriptEngine* asEngine = nullptr;
   asIScriptModule* igModule = nullptr;

   void MessageCallback(const asSMessageInfo *msg, void *param) {
      if (msg->type == asMSGTYPE_WARNING ) {
         IO::Warning(std::to_string(msg->row) + (std::string)":"
               + std::to_string(msg->col) + (std::string)" " + msg->message);
      } else if (msg->type == asMSGTYPE_INFORMATION ) {
         IO::Print(std::to_string(msg->row) + (std::string)":"
               + std::to_string(msg->col) + (std::string)" " + msg->message);
      } else if (msg->type == asMSGTYPE_ERROR) {
         IO::Error(std::to_string(msg->row) + (std::string)":"
               + std::to_string(msg->col) + (std::string)" " + msg->message);
      }
   }

   std::string PreprocessScript(const std::string& filePath, std::unordered_set<std::string>& includedFiles) {
      std::ifstream file(filePath);
      if (!file.is_open()) {
         throw std::runtime_error("Could not open file: " + filePath);
      }

      std::ostringstream processedScript;
      std::string line;

      while (std::getline(file, line)) {
         /*if (line.substr(0, 8) == "#include") {
            std::string includePath = line.substr(9); // Extract path
            includePath = includePath.substr(1, includePath.size() - 2); // Remove quotes

            if (includedFiles.find(includePath) == includedFiles.end()) {
               includedFiles.insert(includePath);
               processedScript << PreprocessScript(includePath, includedFiles);
            } else {*/
               processedScript << line << "\n";
            //}
         //}
      }

      return processedScript.str();
   }
   
   void InitilizeAngelScript() {
      asEngine = asCreateScriptEngine();
      asEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
      RegisterAll();
   }

   void Shutdown() {
      asEngine->ShutDownAndRelease();
   }

   void RegisterAll() {
      RegisterStdString(asEngine);

      RegisterVectors();
      RegisterTransform();
      RegisterIgnitionScript();
      
      RegisterObject();
      RegisterModule();

      RegisterCamera();
      /*
      RegisterModel();
      RegisterShader();
      RegisterMeshRenderer();

      RegisterCollider();
      RegisterRigidbody();


      RegisterInputKeys();
      RegisterInputFunctions();

      */
      RegisterMiscFunctions();
      RegisterExpose();
      RegisterMathFunctions();
   }
}
