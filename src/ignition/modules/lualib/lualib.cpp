#include "lua/lualib.hpp"
#include "utils/io.hpp"
#include "camera.hpp"
#include "input.hpp"

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
         .beginClass<Ignition::Vector2>("Vector2")
            .addConstructor<void(*)(float, float)>()
            .addProperty("x", &Ignition::Vector2::x)
            .addProperty("y", &Ignition::Vector2::y)
         .endClass()

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
         .addFunction("GetCamera", Functions::GetCamera)

         .addFunction("Warning", Ignition::IO::Warning)
         .addFunction("Error", Ignition::IO::Error)
         .addFunction("FatalError", Ignition::IO::FatalError)
         
         .addFunction("GetMouse", Ignition::IO::GetMouse)
         .addFunction("GetMouseUp", Ignition::IO::GetMouseUp)
         .addFunction("GetMouseDown", Ignition::IO::GetMouseDown)
         .addFunction("GetInput", Ignition::IO::GetInput)
         .addFunction("GetInputUp", Ignition::IO::GetInputUp)
         .addFunction("GetInputDown", Ignition::IO::GetInputDown)
         .addFunction("GetAxisHorizontal", Ignition::IO::GetAxisHorizontal)
         .addFunction("GetAxisVertical", Ignition::IO::GetAxisVertical)
         .addFunction("GetMousePosition", Ignition::IO::GetMousePosition)
         
         ;

      luabridge::getGlobalNamespace(state).beginNamespace("Keys")
         .addConstant("a", Ignition::IO::Keys::a)
         .addConstant("b", Ignition::IO::Keys::b)
         .addConstant("c", Ignition::IO::Keys::c)
         .addConstant("d", Ignition::IO::Keys::d)
         .addConstant("e", Ignition::IO::Keys::e)
         .addConstant("f", Ignition::IO::Keys::f)
         .addConstant("g", Ignition::IO::Keys::g)
         .addConstant("h", Ignition::IO::Keys::h)
         .addConstant("i", Ignition::IO::Keys::i)
         .addConstant("j", Ignition::IO::Keys::j)
         .addConstant("k", Ignition::IO::Keys::k)
         .addConstant("l", Ignition::IO::Keys::l)
         .addConstant("m", Ignition::IO::Keys::m)
         .addConstant("n", Ignition::IO::Keys::n)
         .addConstant("o", Ignition::IO::Keys::o)
         .addConstant("p", Ignition::IO::Keys::p)
         .addConstant("q", Ignition::IO::Keys::q)
         .addConstant("r", Ignition::IO::Keys::r)
         .addConstant("s", Ignition::IO::Keys::s)
         .addConstant("t", Ignition::IO::Keys::t)
         .addConstant("u", Ignition::IO::Keys::u)
         .addConstant("v", Ignition::IO::Keys::v)
         .addConstant("w", Ignition::IO::Keys::w)
         .addConstant("x", Ignition::IO::Keys::x)
         .addConstant("y", Ignition::IO::Keys::y)
         .addConstant("z", Ignition::IO::Keys::y)
         .addConstant("one", Ignition::IO::Keys::one)
         .addConstant("two", Ignition::IO::Keys::two)
         .addConstant("three", Ignition::IO::Keys::three)
         .addConstant("four", Ignition::IO::Keys::four)
         .addConstant("five", Ignition::IO::Keys::five)
         .addConstant("six", Ignition::IO::Keys::six)
         .addConstant("seven", Ignition::IO::Keys::seven)
         .addConstant("eight", Ignition::IO::Keys::eight)
         .addConstant("nine", Ignition::IO::Keys::nine)
         .addConstant("zero", Ignition::IO::Keys::zero)
         .addConstant("comma", Ignition::IO::Keys::comma)
         .addConstant("minus", Ignition::IO::Keys::minus)
         .addConstant("space", Ignition::IO::Keys::space)
         .addConstant("period", Ignition::IO::Keys::period)
         .addConstant("semicolon", Ignition::IO::Keys::semicolon)
         .addConstant("apostrophe", Ignition::IO::Keys::apostrophe)
         .addConstant("slash", Ignition::IO::Keys::slash)
         .addConstant("equal", Ignition::IO::Keys::equal)
         .addConstant("up", Ignition::IO::Keys::up)
         .addConstant("down", Ignition::IO::Keys::down)
         .addConstant("left", Ignition::IO::Keys::left)
         .addConstant("right", Ignition::IO::Keys::right)
         .addConstant("enter", Ignition::IO::Keys::enter)
         .addConstant("tab", Ignition::IO::Keys::tab)
         .addConstant("lshift", Ignition::IO::Keys::lshift)
         .addConstant("rshift", Ignition::IO::Keys::rshift)
         .addConstant("esc", Ignition::IO::Keys::esc)
         
         ;
   }

}
