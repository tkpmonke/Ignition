#include "angelscript/angelscript.hpp"
#include "types/transform.hpp"

#include <new>

#define S(x) r = x; assert(r >= 0)

namespace Ignition::Scripting::AngelScript {
   void ConstructVector2(Ignition::Vector2* self) {
      new(self) Ignition::Vector2();
   }

   void ConstructVector2Args(float x, float y, Vector2* self) {
      new(self) Ignition::Vector2(x,y);
   }

   void DestructVector2(Vector2* self) {
      self->~Vector2();
   }

   void ConstructVector3(Ignition::Vector3* self) {
      new(self) Ignition::Vector3();
   }

   void ConstructVector3Args(float x, float y, float z, Vector3* self) {
      new(self) Ignition::Vector3(x,y,z);
   }

   void DestructVector3(Vector3* self) {
      self->~Vector3();
   }
   
   void ConstructVector4(Ignition::Vector4* self) {
      new(self) Ignition::Vector4();
   }

   void ConstructVector4Args(float x, float y, float z, float w, Vector4* self) {
      new(self) Ignition::Vector4(x,y,z,w);
   }

   void DestructVector4(Vector4* self) {
      self->~Vector4();
   }

   void RegisterVectors() {
      int r;

      S(asEngine->RegisterObjectType("Vector2", sizeof(Ignition::Vector2), asOBJ_VALUE));
      S(asEngine->RegisterObjectProperty("Vector2", "float x", asOFFSET(Ignition::Vector2, x)));
      S(asEngine->RegisterObjectProperty("Vector2", "float y", asOFFSET(Ignition::Vector2, y)));
      S(asEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f()",
               asFUNCTION(ConstructVector2), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f(float x, float y)",
               asFUNCTION(ConstructVector2Args), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_DESTRUCT, "void d()",
               asFUNCTION(DestructVector2), asCALL_CDECL_OBJLAST));

      S(asEngine->RegisterObjectType("Vector3", sizeof(Ignition::Vector3), asOBJ_VALUE));
      S(asEngine->RegisterObjectProperty("Vector3", "float x", asOFFSET(Ignition::Vector3, x)));
      S(asEngine->RegisterObjectProperty("Vector3", "float y", asOFFSET(Ignition::Vector3, y)));
      S(asEngine->RegisterObjectProperty("Vector3", "float z", asOFFSET(Ignition::Vector3, z)));
      S(asEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f()",
               asFUNCTION(ConstructVector3), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(float x, float y, float z)",
               asFUNCTION(ConstructVector3Args), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_DESTRUCT, "void d()",
               asFUNCTION(DestructVector3), asCALL_CDECL_OBJLAST));
      
      S(asEngine->RegisterObjectType("Vector4", sizeof(Ignition::Vector4), asOBJ_VALUE));
      S(asEngine->RegisterObjectProperty("Vector4", "float x", asOFFSET(Ignition::Vector4, x)));
      S(asEngine->RegisterObjectProperty("Vector4", "float y", asOFFSET(Ignition::Vector4, y)));
      S(asEngine->RegisterObjectProperty("Vector4", "float z", asOFFSET(Ignition::Vector4, z)));
      S(asEngine->RegisterObjectProperty("Vector4", "float w", asOFFSET(Ignition::Vector4, w)));

      S(asEngine->RegisterObjectBehaviour("Vector4", asBEHAVE_CONSTRUCT, "void f()",
               asFUNCTION(ConstructVector4), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectBehaviour("Vector4", asBEHAVE_CONSTRUCT, "void f(float x, float y, float z, float w)",
               asFUNCTION(ConstructVector4Args), asCALL_CDECL_OBJLAST));
      S(asEngine->RegisterObjectBehaviour("Vector4", asBEHAVE_DESTRUCT, "void d()",
               asFUNCTION(DestructVector4), asCALL_CDECL_OBJLAST));
   }

   void RegisterTransform() {
      int r;
      S(asEngine->RegisterObjectType("Transform", sizeof(Ignition::Transform), asOBJ_REF | asOBJ_NOCOUNT));

      S(asEngine->RegisterObjectMethod("Transform", "void LookAt(Vector3& in pos)", asMETHOD(Ignition::Transform, LookAt), asCALL_THISCALL));

      S(asEngine->RegisterObjectProperty("Transform", "Vector3 postition", asOFFSET(Ignition::Transform, position)));
      S(asEngine->RegisterObjectProperty("Transform", "Vector3 rotation", asOFFSET(Ignition::Transform, position)));
      S(asEngine->RegisterObjectProperty("Transform", "Vector3 scale", asOFFSET(Ignition::Transform, position)));
      
      S(asEngine->RegisterObjectProperty("Transform", "Vector3 forward", asOFFSET(Ignition::Transform, position)));
      S(asEngine->RegisterObjectProperty("Transform", "Vector3 right", asOFFSET(Ignition::Transform, position)));
      S(asEngine->RegisterObjectProperty("Transform", "Vector3 up", asOFFSET(Ignition::Transform, position)));
   }
   void RegisterMiscFunctions() {
      int r;

      S(asEngine->RegisterGlobalFunction("void Print(string msg)",
                  asFUNCTION((void(*)(std::string))Ignition::IO::Print), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("void Print(float msg)",
                  asFUNCTION((void(*)(float))Ignition::IO::Print), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("void Warning(string msg)",
                  asFUNCTION(Ignition::IO::Warning), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("void Error(string msg)",
                  asFUNCTION(Ignition::IO::Error), asCALL_CDECL));
      S(asEngine->RegisterGlobalFunction("void FatalError(string msg)",
                  asFUNCTION(Ignition::IO::FatalError), asCALL_CDECL));
   }
}

#undef S
