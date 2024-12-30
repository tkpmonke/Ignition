#include "modules/script.hpp"
#include "utils/io.hpp"
#include "utils/files.hpp"

#include <filesystem>
#include <vector>
namespace Ignition {
   bool allowScripting = !Ignition::IO::InEditor();
   void Script::Start() {
      if (std::filesystem::exists(Ignition::IO::GetProjectHome()+path) && !std::filesystem::is_directory(Ignition::IO::GetProjectHome()+path)) {
         state = luaL_newstate();
         LoadIgnitionLibrary(state);

         if (luaL_dofile(state, (Ignition::IO::GetProjectHome()+path).data()) != LUA_OK) {
            Ignition::IO::Error("Error Loading Lua Script");
            Ignition::IO::Error(lua_tostring(state, -1));
            lua_pop(state, 1);
            return;
         }

         if (lua_gettop(state)>0) {
            if (lua_istable(state, -1)) {
               module = luabridge::LuaRef::fromStack(state, -1);
               lua_pop(state, 1);

               GetLuaScriptVariables();
               if (module["Start"].isFunction() && allowScripting) {
                  luabridge::setGlobal(state, this->transform, "transform");
                  luabridge::setGlobal(state, this->object, "object");
                  module["Start"]();
               }
               init = true;
            } else {
               return;
            }
         }
         
      }
   }

   float ptime = 0;
   void Script::Update() {
      float time = glfwGetTime();
      float dt = time-ptime;
      ptime = time;

      if (init) {
         luabridge::setGlobal(state, time, "ig_time");
         luabridge::setGlobal(state, dt, "ig_deltaTime");
         luabridge::setGlobal(state, this->transform, "transform");
         luabridge::setGlobal(state, this->object, "object");
         if (module["Update"].isFunction()) { 
            module["Update"]();
         }
      } else if (std::filesystem::exists(Ignition::IO::GetProjectHome()+path)
         && !std::filesystem::is_directory(Ignition::IO::GetProjectHome()+path)) {
         Start();
      }
   }

   void Script::Shutdown() {
      if (module["Shutdown"].isFunction()) 
         module["Shutdown"]();

      lua_close(state);
      state = nullptr;
   }

   void Script::Serialize() {
      Ignition::IO::WriteString(path);
      Ignition::IO::Write16(variables.size());

      for (auto& v : variables) {
         Ignition::IO::WriteString(v.name);
         Ignition::IO::Write8((int)v.type);
         switch(v.type) {
            case LuaData::Float:
               Ignition::IO::WriteFloat(v.f);
               break;
            case LuaData::String:
               Ignition::IO::WriteString(v.s);
               break;
            case LuaData::Boolean:
               Ignition::IO::Write8(v.b);
               break;
         }
      }
   }

   void Script::Deserialize() {
      this->path = Ignition::IO::ReadString();
      int count = Ignition::IO::Read16();

      for (int i = 0; i < count; ++i) {
         LuaData v;
         v.name = Ignition::IO::ReadString();
         v.type = (LuaData::Type)Ignition::IO::Read8();
         switch(v.type) {
            case LuaData::Float:
               v.f = Ignition::IO::ReadFloat();
               break;
            case LuaData::String:
               v.s = Ignition::IO::ReadString();
               break;
            case LuaData::Boolean:
               v.b = Ignition::IO::Read8();
               break;
         }

         variables.push_back(v);
      }
   }

   void Script::GetLuaScriptVariables() {
      if (Ignition::IO::InEditor()) {
         std::vector<LuaData> o;
         this->variables.clear();
         std::string name;

         for (luabridge::Iterator it(module); !it.isNil(); ++it) {
            name = it.key().tostring();
            auto value = it.value();

            if (value.isBool()) {
               o.push_back({
                  .b = value.cast<bool>(), 
                  .type = LuaData::Boolean, 
                  .name = name
               });
            } else if (value.isNumber()) {
               o.push_back({
                  .f = value.cast<float>(), 
                  .type = LuaData::Float, 
                  .name = name
               });
            } else if (value.isString()) {
               o.push_back({
                  .s = value.cast<std::string>(),
                  .type = LuaData::String, 
                  .name = name
               });
            }
         }

         
         this->variables = o;
      } else {
         for (auto& v : variables) {
            switch (v.type) {
               case LuaData::Boolean:
                  module[v.name] = v.b;
                  break;
               case LuaData::Float:
                  module[v.name] = v.f;
                  break;
               case LuaData::String:
                  module[v.name] = v.s;
                  break;
            }
         }
      }
      
      for (auto& v : variables) {
         Ignition::IO::DebugPrint(v.name);
         
         switch(v.type) {
            case LuaData::Boolean:
               Ignition::IO::DebugPrint(v.b ? "true" : "false");
               break;
            case LuaData::Float:
               Ignition::IO::DebugPrint(std::to_string(v.f));
               break;
            case LuaData::String:
               Ignition::IO::DebugPrint(v.s);
               break;
         }
      }
   }
}
