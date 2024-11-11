#include "lua/lualib.hpp"

#include "camera.hpp"

namespace Ignition::Scripting::Lua {
   lua_State* state;

   void LoadIgnitionLibrary(lua_State* state) {
      luaL_openlibs(state);

      RegisterFunctionsAndClasses(state);
   }

   void Functions::Shutdown() {
      Ignition::MainCamera::camera->window->Shutdown();
   }

   void Functions::Reboot() {
      Ignition::MainCamera::camera->window->Restart();
   }

   Ignition::Camera* Functions::GetCamera() { return Ignition::MainCamera::camera;}
   int Functions::CreateObject() { return 0; }

   void RegisterFunctionsAndClasses(lua_State* state) {
      luabridge::getGlobalNamespace(state)

         .beginClass<Ignition::Vector3>("Vector3")
            .addConstructor<void(*)(float, float, float)>()
            .addProperty("x", &Ignition::Vector3::x)
            .addProperty("y", &Ignition::Vector3::y)
            .addProperty("z", &Ignition::Vector3::z)
         .endClass()

         .beginClass<Ignition::Vector4>("Vector4")
            .addConstructor<void(*)(float, float, float, float)>()
            .addProperty("x", &Ignition::Vector4::x)
            .addProperty("y", &Ignition::Vector4::y)
            .addProperty("z", &Ignition::Vector4::z)
            .addProperty("w", &Ignition::Vector4::w)
         .endClass()

         .beginClass<Ignition::Transform>("Transform")
            .addConstructor<void(*)(Ignition::Vector3)>()
            .addConstructor<void(*)(Ignition::Vector3, Ignition::Vector3)>()
            .addConstructor<void(*)(Ignition::Vector3, Ignition::Vector3, Ignition::Vector3)>()
            .addProperty("position", &Ignition::Transform::position)
            .addProperty("rotation", &Ignition::Transform::rotation)
            .addProperty("scale", &Ignition::Transform::scale)
            .addProperty("forward", &Ignition::Transform::forward)
            .addProperty("right", &Ignition::Transform::right)
            .addProperty("up", &Ignition::Transform::up)
         .endClass()

         .beginClass<Ignition::Camera>("Camera")
            .addProperty("fov", &Ignition::Camera::fov)
            .addProperty<Ignition::Transform>("transform", &Ignition::Camera::transform)
         .endClass()

         .beginClass<Ignition::Object>("Object")
            .addProperty("name", &Ignition::Object::name)
            .addProperty("tag", &Ignition::Object::tag)
            .addProperty("enabled", &Ignition::Object::enabled)
            .addProperty<Ignition::Transform>("transform", &Ignition::Object::transform)
         .endClass()

         .addFunction("Poweroff", Functions::Shutdown)
         .addFunction("Reboot", Functions::Reboot)
         .addFunction("CreateObject", Functions::CreateObject)
         .addFunction("GetCamera", Functions::GetCamera); 
   }

}
