#include "lua/lualib.hpp"
#include "utils/io.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "modules/rendering/meshrenderer.hpp"
#include "vels/vels.hpp"
#include "modules/physics/rigidbody.hpp"

namespace Ignition::Scripting::Lua {
   lua_State* state;

   void LoadIgnitionLibrary(lua_State* state) {
      luaL_openlibs(state);

      RegisterFunctionsAndClasses(state);
   }

   void Functions::Shutdown() {
      Ignition::mainCamera->window->Shutdown();
   }

   void Functions::Reboot() {
      Ignition::mainCamera->window->Restart();
   }

   Ignition::Camera* Functions::GetCamera() { return Ignition::mainCamera;}
   int Functions::CreateObject() { return 0; }

   void RegisterFunctionsAndClasses(lua_State* state) {
      luabridge::getGlobalNamespace(state)
         .beginNamespace("ig")
         .beginClass<Ignition::Vector2>("Vector2")
            .addConstructor<void(*)(float)>()
            .addConstructor<void(*)(float, float)>()
            .addProperty("x", &Ignition::Vector2::x)
            .addProperty("y", &Ignition::Vector2::y)
         .endClass()

         .beginClass<Ignition::Vector3>("Vector3")
            .addConstructor<void(*)(float)>()
            .addConstructor<void(*)(float, float, float)>()
            .addProperty("x", &Ignition::Vector3::x)
            .addProperty("y", &Ignition::Vector3::y)
            .addProperty("z", &Ignition::Vector3::z)
            .addFunction("__add", +[](Vector3* a, Vector3& b){return *a+b;})
            .addFunction("__mul", +[](Vector3* a, Vector3& b){return *a*b;})
         .endClass()
         
         .beginClass<JPH::Vec3>("Vec3")
            .addProperty("x", &Vec3::GetX, &Vec3::SetX)
            .addProperty("y", &Vec3::GetY, &Vec3::SetY)
            .addProperty("z", &Vec3::GetZ, &Vec3::SetZ)
         .endClass()

         .addFunction("ig_to_jph", +[](Vector3* a){return new Vec3(a->x,a->y,a->z);})

         .beginClass<Ignition::Vector4>("Vector4")
            .addConstructor<void(*)(float)>()
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
            .addFunction("LookAt", &Ignition::Transform::LookAt)
            .addProperty("position", &Ignition::Transform::position)
            .addProperty("rotation", &Ignition::Transform::rotation)
            .addProperty("scale", &Ignition::Transform::scale)
            .addProperty("forward", &Ignition::Transform::forward)
            .addProperty("right", &Ignition::Transform::right)
            .addProperty("up", &Ignition::Transform::up)
         .endClass()

         .beginClass<Ignition::Module>("Module")
            .addFunction("Start", &Ignition::Module::Start)
            .addFunction("Update", &Ignition::Module::Update)
            .addFunction("LateUpdate", &Ignition::Module::LateUpdate)
            .addFunction("Shutdown", &Ignition::Module::Shutdown)
            .addFunction("RunsInEditor", &Ignition::Module::runs_in_editor)
            //.addFunction("ModType", &Ignition::Module::mod_type)
            .addProperty("enabled", &Ignition::Module::enabled)
            .addProperty("transform", &Ignition::Module::transform)
         .endClass()

         .beginClass<Ignition::Model>("Model")
            .addProperty("indices", &Ignition::Model::indices)
            .addProperty("vertices", &Ignition::Model::vertices)
            .addProperty("normals", &Ignition::Model::normals)
            .addProperty("uv", &Ignition::Model::uv)
         .endClass()

         .beginNamespace("ShaderType")
            .addProperty("Lit", +[](){return static_cast<int>(Ignition::Rendering::ShaderType::Lit);})
            .addProperty("Unlit", +[](){return static_cast<int>(Ignition::Rendering::ShaderType::Unlit);})
            .addProperty("Compute", +[](){return static_cast<int>(Ignition::Rendering::ShaderType::Compute);})
         .endNamespace()

         .beginClass<Ignition::Rendering::Shader>("Shader")
            .addConstructor<void(*)(std::string, std::string, int)>()
            .addConstructor<void(*)(std::string, int)>()
            .addFunction("SetFloat", &Ignition::Rendering::Shader::SetFloat)
            .addFunction("SetInt", &Ignition::Rendering::Shader::SetInt)
            .addFunction("SetBool", &Ignition::Rendering::Shader::SetBool)
            .addFunction("SetDouble", &Ignition::Rendering::Shader::SetDouble)
            .addFunction("SetMatrix3", &Ignition::Rendering::Shader::SetMatrix3)
            .addFunction("SetMatrix4", &Ignition::Rendering::Shader::SetMatrix4)
            .addFunction("SetVector2", &Ignition::Rendering::Shader::SetVec2)
            .addFunction("SetVector3", &Ignition::Rendering::Shader::SetVec3)
            .addFunction("SetVector4", &Ignition::Rendering::Shader::SetVec4)
            .addProperty("color", &Ignition::Rendering::Shader::color)
            .addProperty("intensity", &Ignition::Rendering::Shader::intensity)
         .endClass()

         .deriveClass<Ignition::Rendering::MeshRenderer, Ignition::Module>("MeshRenderer")
            .addConstructor<void(*)()>()
            .addFunction("LoadShader", &Ignition::Rendering::MeshRenderer::LoadShader)
            .addFunction("LoadModel", &Ignition::Rendering::MeshRenderer::LoadModel)
            
            .addProperty("shader", &Ignition::Rendering::MeshRenderer::shader)
            .addProperty("model", &Ignition::Rendering::MeshRenderer::model, false)
         .endClass()

         .beginClass<vels::Collider>("Collider")
            .addConstructor<void(*)()>()
            .addFunction("CreateCube", &vels::Collider::CreateCube)
            .addFunction("CreateSphere", &vels::Collider::CreateSphere)
            .addFunction("CreateEmpty", &vels::Collider::CreateEmpty)
         .endClass()

         .deriveClass<vels::Rigidbody, Ignition::Module>("internal_vels_rigidbody")
            .addFunction("AddCollider", &vels::Rigidbody::AddCollider)
            .addFunction("AddForce", &vels::Rigidbody::AddForce)
            .addProperty("mass", &vels::Rigidbody::mass)
            .addProperty("velocity", &vels::Rigidbody::velocity)
            .addProperty("static", &vels::Rigidbody::_static)
            .addProperty("trigger", &vels::Rigidbody::trigger)
            .addProperty("collider", &vels::Rigidbody::collider)
         .endClass()

         .deriveClass<Ignition::Physics::Rigidbody,vels::Rigidbody>("Rigidbody")
            .addConstructor<std::shared_ptr<Ignition::Physics::Rigidbody>(*)()>()
            .addFunction("Start", &Ignition::Physics::Rigidbody::Start)
         .endClass()

         .beginClass<Ignition::Object>("Object")
            .addProperty("name", &Ignition::Object::name)
            .addProperty("tag", &Ignition::Object::tag)
            .addProperty("enabled", &Ignition::Object::enabled)
            .addData("id", &Ignition::Object::id, false)
            .addProperty<Ignition::Transform>("transform", &Ignition::Object::transform)
            .addProperty<Ignition::Object*>("parent", &Ignition::Object::parent)
            .addFunction("AddChild", &Ignition::Object::AddChild)
            .addFunction("AddModule", &Ignition::Object::AddModule)
            .addFunction("GetModule", &Ignition::Object::GetModule)
         .endClass()

         .deriveClass<Ignition::Camera, Ignition::Object>("Camera")
            .addProperty("fov", &Ignition::Camera::fov)
         .endClass()

         .addFunction("Poweroff", Functions::Shutdown)
         .addFunction("Reboot", Functions::Reboot)
         .addFunction("CreateObject", Functions::CreateObject)
         .addFunction("GetCamera", Functions::GetCamera)

         .addFunction("Print", Ignition::IO::Print)
         .addFunction("Warning", Ignition::IO::Warning)
         .addFunction("Error", Ignition::IO::Error)
         .addFunction("FatalError", Ignition::IO::FatalError)
         
         .addFunction("GetScrollX", Ignition::IO::GetScrollX)
         .addFunction("GetScrollY", Ignition::IO::GetScrollY)
         .addFunction("GetMouse", Ignition::IO::GetMouse)
         .addFunction("GetMouseUp", Ignition::IO::GetMouseUp)
         .addFunction("GetMouseDown", Ignition::IO::GetMouseDown)
         .addFunction("GetInput", Ignition::IO::GetInput)
         .addFunction("GetInputUp", Ignition::IO::GetInputUp)
         .addFunction("GetInputDown", Ignition::IO::GetInputDown)
         .addFunction("GetAxisHorizontal", Ignition::IO::GetAxisHorizontal)
         .addFunction("GetAxisVertical", Ignition::IO::GetAxisVertical)
         .addFunction("GetMousePosition", Ignition::IO::GetMousePosition)
         
         .beginNamespace("Keys")
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
         .endNamespace()
         
         
         .endNamespace();
   }

}
