#include "types/transform.hpp"

namespace Ignition {
   
   void Transform::UpdateVectors()
   {
      Vector3 euler = glm::eulerAngles(this->rotation);
      float pitch = euler.y;
      float yaw = euler.z;
      this->forward.x = cos(yaw) * cos(pitch);
      this->forward.y = sin(pitch);
      this->forward.z = sin(yaw) * cos(pitch);
      this->forward = glm::normalize(this->forward);
      this->right = glm::normalize(glm::cross(this->forward, Vector3(0,1,0)));
      this->up = glm::normalize(glm::cross(this->right, this->forward));

   }
}
