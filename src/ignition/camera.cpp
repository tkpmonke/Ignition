#include "camera.hpp"
#include "modules/rendering/meshrenderer.hpp"
#include "modules/rendering/light.hpp"

namespace Ignition {
   Camera* mainCamera;
   void Camera::MakeMainCamera()
   {
      Ignition::mainCamera = this;
   }

   Matrix4 Camera::ViewProjectionMatrix() 
   {
      return ProjectionMatrix() * ViewMatrix();
   }

   Matrix4 Camera::ViewMatrix() 
   {
      Matrix4 view = glm::lookAt(this->transform.position, this->transform.position + this->transform.forward, this->transform.up);
      return view;
   }

   Matrix4 Camera::ProjectionMatrix() 
   {
      if (size == Vector2(0))
      {

         glfwGetWindowSize((GLFWwindow*)*this->window, (int*)&size.x, (int*)&size.y);
      }
      Matrix4 projection;
      
      if (this->projectionMode == Perspective) {
         projection = glm::perspective(glm::radians(this->fov), (float)size.x/(float)size.y, this->clippingPlanes.min, this->clippingPlanes.max);
      } else {
         projection = glm::ortho(0.f, (float)this->size.x, 0.f, (float)this->size.y, this->clippingPlanes.min, this->clippingPlanes.max); 
      }
      return projection;
   }

   void Camera::BeginRender()
   {
      this->transform.UpdateVectors();
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.f, 0.f, 0.f, 1.f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   void Camera::EndRender(bool gui)
   {
      Rendering::currentProgram = std::numeric_limits<int>::max();
      Rendering::currentVao = std::numeric_limits<int>::max();
      viewProj = Matrix4(0);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDisable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT);
      if (!gui) {
         glUseProgram(window->s.program);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, window->color);
         //window->s.SetInt(0, "tex");

         glBindVertexArray(window->vao);
         glDrawArrays(GL_TRIANGLES, 0, 6);
          
         glfwSwapBuffers((GLFWwindow*)*this->window);
         
      }

      glfwPollEvents();
      
   }

   void Camera::EndGUI() {
      
      glfwSwapBuffers((GLFWwindow*)*this->window);
   }
}
