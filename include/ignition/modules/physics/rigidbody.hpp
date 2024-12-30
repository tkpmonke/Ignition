#pragma once

#include "physics.hpp"
#include "module.hpp"
#include "collider.hpp"

namespace Ignition::Physics {
   class Rigidbody : public Module {
   public:
      CREATE_MODULE("Rigidbody");

      float mass = 1;
      float bounciness = 0.1f;
      float drag = 0.1f;

      Vector3 velocity = Vector3(0, 0, 0);

      bool _static = false;
      bool trigger = false;

      Collider collider;

      void AddForce(Vector3, int);
      void AddCollider(Collider);

      void FixedUpdate() override;
      void LateFixedUpdate() override;
      void Shutdown() override;

      void Serialize() override;
      void Deserialize() override;

      Rigidbody() = default;

   private:
      Vector3 QuatToEuler(JPH::Quat);
      JPH::Quat EulerToQuat(Vector3);

      Vec3 Vector3ToVec3(Vector3);
      Vector3 Vec3ToVector3(Vec3);

      Body* body = nullptr;
   };
}
