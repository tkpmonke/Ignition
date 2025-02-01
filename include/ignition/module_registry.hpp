#pragma once

#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

#include "module.hpp"

#define IGMODULE(x) \
   public: \
   x() { \
      BindProperties(); \
   } \
   static std::shared_ptr<x> _make_shared_ptr() { \
      return std::make_shared<x>(); \
   } \
   const std::string _get_name() override { \
      return #x; \
   } \
   static void _register() { \
      Ignition::ModuleRegistry::GetInstance() \
         .RegisterModule(#x, &_make_shared_ptr); \
   } \
   private: \
   struct AutoRegister { \
      AutoRegister() { x::_register(); } \
   }; \
   static inline AutoRegister _auto_registrar; 

#define IGRUNS_IN_EDITOR() bool _runs_in_editor() const override { return true; }


namespace Ignition {
   class ModuleRegistry {
   public:
      struct ModuleInfo {
         std::string name;
         std::function<std::shared_ptr<Module>()> createModuleFunc;
      };

      static ModuleRegistry& GetInstance() {
         static ModuleRegistry instance;
         return instance;
      }

      void RegisterModule(const std::string& name, std::function<std::shared_ptr<Module>()> createModuleFunc) {
         modMap[name] = {name, createModuleFunc };
      }

      std::shared_ptr<Module> CreateModule(const std::string& name) {
         if (modMap.find(name) != modMap.end())
            return modMap[name].createModuleFunc();
         return nullptr;
      }

   private:
      std::unordered_map<std::string, ModuleInfo> modMap;
   };
   // shorthand version, like godot's ClassDB
   using ModuleDB = ModuleRegistry;
}
