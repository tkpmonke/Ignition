#include "modules/physics/rigidbody.hpp"
#include "utils/files.hpp"

namespace Ignition::Physics {

   Vector3 Rigidbody::QuatToEuler(JPH::Quat q) {
      glm::quat a = glm::quat::wxyz(q.GetW(),q.GetX(),q.GetY(),q.GetZ());
      return glm::degrees(glm::eulerAngles(a));
   }

   JPH::Quat Rigidbody::EulerToQuat(Vector3 v) {
      glm::quat a = glm::quat(glm::radians(v));
      return JPH::Quat(a.x,a.y,a.z,a.w);
   }

   Vec3 Rigidbody::Vector3ToVec3(Vector3 v) {
      return Vec3(v.x,v.y,v.z);
   }

   Vector3 Rigidbody::Vec3ToVector3(Vec3 v) {
      return Vector3(v.GetX(), v.GetY(), v.GetZ());
   }

   void Rigidbody::Start() {
      Init(physicsWorld);
   }

   void Rigidbody::FixedUpdate() {
      position = Vector3ToVec3(transform->position);
      rotation = EulerToQuat(transform->rotation);
      UpdateFixed();
   }

   void Rigidbody::LateFixedUpdate() {
      PostUpdateFixed();

      transform->position = Vec3ToVector3(position);
      transform->rotation = QuatToEuler(rotation);
   }

   void Rigidbody::Shutdown() {
      Delete();
   }

   void Rigidbody::Serialize() {
      IO::Write8(this->trigger);
      IO::Write8(this->_static);
      IO::WriteFloat(this->mass);

      IO::Write8(this->collider.shapeType);

      switch(this->collider.shapeType) {
         case(vels::Collider::Cube): {
            Vec3 v = this->collider.shapeVariables.size;
            IO::WriteVector3(Vector3(v.GetZ(), v.GetY(), v.GetZ()));
            break;
         }
         case(vels::Collider::Sphere): {
            IO::WriteFloat(this->collider.shapeVariables.radius);
            break;
         }
         default:
            break;
      }
   }

   void Rigidbody::Deserialize() {
      this->trigger = IO::Read8();
      this->_static = IO::Read8();
      this->mass = IO::ReadFloat();

      if (!Ignition::IO::InEditor()) {
         switch(IO::Read8()) {
            default:
            case(vels::Collider::Empty): {
               this->collider.CreateEmpty();
               IO::DebugPrint("Loaded Empty Rigidbody");
               break;
            }
            case(vels::Collider::Cube): {
               Vector3 v = IO::ReadVector3();
               this->collider.CreateCube(Vec3(v.x, v.y, v.z));
               IO::DebugPrint((std::string)"Loaded Cube > " + std::to_string(v.x) + 
                     (std::string)" " + std::to_string(v.y) + 
                     (std::string)" " + std::to_string(v.z));
               break;
            }
            case(vels::Collider::Sphere):
               this->collider.CreateSphere(IO::ReadFloat());
               IO::DebugPrint("Loaded Sphere");
               break;
         }
         AddCollider(this->collider);
      } else {
         this->collider.shapeType = (vels::Collider::ShapeType)IO::Read8();

         if (this->collider.shapeType == vels::Collider::Sphere) {
            this->collider.shapeVariables.radius = IO::ReadFloat();
         }
         if (this->collider.shapeType == vels::Collider::Cube) {
            this->collider.shapeVariables.size = Vector3ToVec3(IO::ReadVector3());
         }
      }
   }
}
