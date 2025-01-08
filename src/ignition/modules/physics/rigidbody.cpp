
#include "utils/files.hpp"
#include "modules/physics/rigidbody.hpp"

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

   void Rigidbody::FixedUpdate() {
      BodyInterface& bodyInterface = physicsWorld->physicsSystem.GetBodyInterface();

      if (body != nullptr ) {
         if (!_static) {
            body->SetLinearVelocity(Vector3ToVec3(this->velocity));
            body->GetMotionProperties()->SetLinearDamping(drag);
         }

         body->SetIsSensor(this->trigger);
         bodyInterface.SetGravityFactor(body->GetID(), this->mass);
         bodyInterface.SetRestitution(body->GetID(), bounciness);
         bodyInterface.SetFriction(body->GetID(), drag);
         bodyInterface.SetPositionAndRotation(
            body->GetID(), 
            Vector3ToVec3(this->transform->position), 
            EulerToQuat(this->transform->rotation), 
            EActivation::Activate);
      }
   }

   void Rigidbody::LateFixedUpdate() {
      BodyInterface& bodyInterface = physicsWorld->physicsSystem.GetBodyInterface();
      if (body != nullptr/* && !_static*/) {
         this->velocity = Vec3ToVector3(bodyInterface.GetLinearVelocity(body->GetID()));
         transform->position = Vec3ToVector3(bodyInterface.GetCenterOfMassPosition(body->GetID()));
         transform->rotation = QuatToEuler(bodyInterface.GetRotation(body->GetID()));
      }
   }

   void Rigidbody::Shutdown() {
      if (body != nullptr) {
         BodyInterface& bodyInterface = physicsWorld->physicsSystem.GetBodyInterface();
         bodyInterface.RemoveBody(body->GetID());
         bodyInterface.DestroyBody(body->GetID());
      }
   }

   void Rigidbody::Serialize() {
      IO::Write8(this->trigger);
      IO::Write8(this->_static);
      IO::WriteFloat(this->mass);
      IO::WriteFloat(this->bounciness);
      IO::WriteFloat(this->drag);

      IO::Write8(this->collider.shapeType);

      switch(this->collider.shapeType) {
         case(Collider::Cube): {
            Vec3 v = this->collider.shapeVariables.size;
            IO::WriteVector3(Vector3(v.GetX(), v.GetY(), v.GetZ()));
            break;
         }
         case(Collider::Sphere): {
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
      this->bounciness = IO::ReadFloat();
      this->drag = IO::ReadFloat();

      if (!Ignition::IO::InEditor()) {
         switch(IO::Read8()) {
            default:
            case(Collider::Empty): {
               this->collider.CreateEmpty();
               IO::DebugPrint("Loaded Empty Rigidbody");
               break;
            }
            case(Collider::Cube): {
               Vector3 v = IO::ReadVector3();
               this->collider.CreateCube(Vec3(v.x, v.y, v.z));
               IO::DebugPrint((std::string)"Loaded Cube > " + std::to_string(v.x) + 
                     (std::string)" " + std::to_string(v.y) + 
                     (std::string)" " + std::to_string(v.z));
               break;
            }
            case(Collider::Sphere):
               this->collider.CreateSphere(IO::ReadFloat());
               IO::DebugPrint("Loaded Sphere > " + std::to_string(this->collider.shapeVariables.radius));
               break;
            case(Collider::Mesh):
               auto ptr = object->GetMeshRenderer();
               if (ptr != nullptr) 
                  this->collider.CreateMesh(&ptr->model.vertices, &ptr->model.indices);
               break;
         }
         AddCollider(this->collider);
      } else {
         this->collider.shapeType = (Collider::ShapeType)IO::Read8();

         if (this->collider.shapeType == Collider::Sphere) {
            this->collider.shapeVariables.radius = IO::ReadFloat();
         }
         if (this->collider.shapeType == Collider::Cube) {
            this->collider.shapeVariables.size = Vector3ToVec3(IO::ReadVector3());
         }
      }
   }

   void Rigidbody::AddCollider(Collider c) {
      BodyInterface& bodyInterface = physicsWorld->physicsSystem.GetBodyInterface();
         
      if (body != nullptr) {
         bodyInterface.RemoveBody(body->GetID());
         bodyInterface.DestroyBody(body->GetID());
         body = nullptr;
      }
      this->collider = c;
      EMotionType motionType = _static ? EMotionType::Static : EMotionType::Dynamic;
      int layer = _static ? Layers::NonMoving : Layers::Moving;

      if (collider.shape == nullptr) {
         std::cerr << "Collider shape is null!\n";
         return;
      }
      
      BodyCreationSettings settings(
          collider.shape,
          Vector3ToVec3(transform->position),
          EulerToQuat(transform->rotation),
          motionType,
          layer
      );
      
      body = bodyInterface.CreateBody(settings);
      if (body == nullptr) {
         std::cerr << "Failed to create body!\n";
         return;
      }
      bodyInterface.AddBody(body->GetID(), EActivation::Activate);
   }

   void Rigidbody::AddForce(Vector3 force, int type) {
         BodyInterface& bodyInterface = physicsWorld->physicsSystem.GetBodyInterface();
         bodyInterface.ActivateBody(body->GetID());

         Vector3 vel = Vec3ToVector3(bodyInterface.GetLinearVelocity(body->GetID()));

         if (type == 0) {
            vel += force;
         } else {
            vel += force*physicsWorld->deltaTime;
         }

         this->velocity = vel;
      }
}
