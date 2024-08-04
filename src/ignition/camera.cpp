#include "camera.hpp"
#include "components/rendering/meshrenderer.hpp"

namespace Ignition::MainCamera {
   Camera* camera;
}

namespace Ignition {
   void Camera::MakeMainCamera()
   {
      Ignition::MainCamera::camera = this;
   }

   Matrix4 Camera::view_projection() 
   {
      this->transform.UpdateVectors();

      Matrix4 view = glm::lookAt(this->transform.position, this->transform.position + this->transform.forward, this->transform.up);

      
      int width, height;
      glfwGetWindowSize((GLFWwindow*)*this->window, &width, &height);
      Matrix4 projection = glm::perspective(glm::radians(this->fov), (float)width/(float)height, this->clipping_planes.min, this->clipping_planes.max);

      return projection * view;
   }

   void Camera::BeginRender()
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   void Camera::EndRender()
   {
      glfwSwapBuffers((GLFWwindow*)*this->window);
      Rendering::currentProgram = 99;
   }
}
