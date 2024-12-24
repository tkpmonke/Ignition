#pragma once

#include "physics.hpp"
#include "../../module.hpp"

namespace Ignition::Physics {
   class Rigidbody : public Module, public vels::Rigidbody {
   public:
      CREATE_MODULE("Rigidbody");

      void Start() override;
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
   };
}
