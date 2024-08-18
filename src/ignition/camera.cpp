#include "camera.hpp"
#include "modules/rendering/meshrenderer.hpp"

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

      if (size == Vector2(0))
      {

         glfwGetWindowSize((GLFWwindow*)*this->window, (int*)&size.x, (int*)&size.y);
      }
      Matrix4 projection = glm::perspective(glm::radians(this->fov), (float)size.x/(float)size.y, this->clipping_planes.min, this->clipping_planes.max);

      return projection * view;
   }

   void Camera::BeginRender()
   {
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   void Camera::EndRender(bool gui)
   {
      Rendering::currentProgram = 9999;
      Rendering::currentVao = 9999;
      viewProj = Matrix4(0);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDisable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT);
      if (!gui) {
         
         glUseProgram(window->s.program);
         glBindTexture(GL_TEXTURE_2D, window->color);
         glBindVertexArray(window->vao);
         glDrawArrays(GL_TRIANGLES, 0, 6);
          
         glfwSwapBuffers((GLFWwindow*)*this->window);
      }
   }

   void Camera::EndGUI() {
      
      glfwSwapBuffers((GLFWwindow*)*this->window);
   }
}
