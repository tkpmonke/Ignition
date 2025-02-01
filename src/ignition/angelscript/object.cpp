#include "angelscript/angelscript.hpp"

#include "object.hpp"
#include "camera.hpp"
#include "scene.hpp"

#define S(x) r = x; assert(r >= 0)

Ignition::Object* CreateObject() {
   Ignition::scene.CreateObject();
   return &Ignition::scene.GetObjects()->at(Ignition::scene.GetObjects()->size()-1);
}

Ignition::Camera* GetMainCamera() {
   return Ignition::mainCamera;
}


namespace Ignition::Scripting::AngelScript {
   void RegisterObject() { 
      int r;

      S(asEngine->RegisterObjectProperty("Object", "Transform transform", asOFFSET(Ignition::Object, transform)));
      S(asEngine->RegisterObjectProperty("Object", "string name", asOFFSET(Ignition::Object, name)));
      S(asEngine->RegisterObjectProperty("Object", "string tag", asOFFSET(Ignition::Object, tag)));
      S(asEngine->RegisterObjectProperty("Object", "bool enabled", asOFFSET(Ignition::Object, enabled)));
      
      S(asEngine->RegisterObjectMethod("Object", "Module@ GetModule(string)", asMETHOD(Ignition::Object, GetModule), asCALL_THISCALL));
      S(asEngine->RegisterObjectMethod("Object", "Rigidbody@ GetRigidbody()", asMETHOD(Ignition::Object, GetRigidbody), asCALL_THISCALL));

      S(asEngine->RegisterGlobalFunction("Object@ CreateObject()", asFUNCTION(CreateObject), asCALL_CDECL));
      
   }

   void RegisterCamera() {
      int r;

      S(asEngine->RegisterEnum("ProjectionMode"));
      S(asEngine->RegisterEnumValue("ProjectionMode", "Perspective", Ignition::Camera::ProjectionMode::Perspective));
      S(asEngine->RegisterEnumValue("ProjectionMode", "Orthographic", Ignition::Camera::ProjectionMode::Orthographic));

      S(asEngine->RegisterObjectType("Camera", sizeof(Ignition::Camera), asOBJ_REF | asOBJ_NOCOUNT));

      S(asEngine->RegisterObjectProperty("Camera", "float fov", asOFFSET(Ignition::Camera, fov)));
      S(asEngine->RegisterObjectProperty("Camera", "ProjectionMode projectionMode", asOFFSET(Ignition::Camera, projectionMode)));
      S(asEngine->RegisterObjectProperty("Camera", "Transform transform", asOFFSET(Ignition::Camera, transform)));
      S(asEngine->RegisterObjectProperty("Camera", "string name", asOFFSET(Ignition::Camera, name)));
      S(asEngine->RegisterObjectProperty("Camera", "string tag", asOFFSET(Ignition::Camera, tag)));
      S(asEngine->RegisterObjectProperty("Camera", "bool enabled", asOFFSET(Ignition::Camera, enabled)));

      S(asEngine->RegisterObjectMethod("Object", "Camera@ opCast()", asFUNCTION((refCast<Ignition::Object, Ignition::Camera>)), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectMethod("Camera", "Object@ opImplCast()", asFUNCTION((refCast<Ignition::Camera, Ignition::Object>)), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectMethod("Object", "const Camera@ opCast()", asFUNCTION((refCast<Ignition::Object, Ignition::Camera>)), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectMethod("Camera", "const Object@ opImplCast()", asFUNCTION((refCast<Ignition::Camera, Ignition::Object>)), asCALL_CDECL_OBJLAST));

      S(asEngine->RegisterGlobalFunction("Camera@ GetMainCamera()", asFUNCTION(GetMainCamera), asCALL_CDECL));
   }

   void RegisterModule() {
      int r;
      S(asEngine->RegisterObjectType("Module", sizeof(Ignition::Module), asOBJ_REF | asOBJ_NOCOUNT));

      S(asEngine->RegisterObjectProperty("Module", "Transform@ transform", asOFFSET(Ignition::Module, transform)));
      S(asEngine->RegisterObjectProperty("Module", "Object@ object", asOFFSET(Ignition::Module, transform)));
      S(asEngine->RegisterObjectProperty("Module", "bool enabled", asOFFSET(Ignition::Module, enabled)));

   }
}
