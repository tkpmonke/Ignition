#pragma once
#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Ignition {
   using Vector1 = glm::vec1;
   using Vector2 = glm::vec2;
   using Vector3 = glm::vec3;
   using Vector4 = glm::vec4;

   using Quaternion = glm::quat;
   using Quat = glm::quat;
   
   template <glm::length_t X, glm::length_t Y, typename T, glm::qualifier Q>
   using Matrix = glm::mat<X,Y,T,Q>;
   using Matrix3 = glm::mat3;
   using Matrix4 = glm::mat4;
   
   class Transform {
   public:
      Vector3 position;
      Vector3 rotation;
      Vector3 scale = Vector3(1,1,1);
      
      Vector3 forward;
      Vector3 right;
      Vector3 up;

      Transform() = default;
      Transform(Vector3 position) : position(position) {}
      Transform(Vector3 position, Vector3 rotation) : position(position), rotation(rotation) {}
      Transform(Vector3 position, Vector3 rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale) {}
      
      void UpdateVectors();

      void LookAt(Vector3);

   };
}
