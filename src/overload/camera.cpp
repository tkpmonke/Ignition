#include "camera.hpp"

namespace Overload::MainCamera {
   Camera* camera;
}

namespace Overload {
   void Camera::MakeMainCamera()
   {
      Overload::MainCamera::camera = this;
   }

   Matrix4 Camera::view_projection() 
   {
      this->transform.UpdateVectors();

      Matrix4 view = glm::lookAt(this->transform.position, this->transform.position + this->transform.forward, this->transform.up);
      
      int width, height;
      glfwGetWindowSize((GLFWwindow*)this->window, &width, &height);
      Matrix4 projection = glm::perspective(this->fov, (float)width/(float)height, this->clipping_planes.min, this->clipping_planes.max);

      return projection * view;
   }
}
