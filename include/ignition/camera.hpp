#pragma once
#include "object.hpp"
#include "window.hpp"

#include <glm/mat4x4.hpp>

namespace Ignition {
   class Camera : public Object {
   public:
      float fov;
      struct {
         float min, max;
      } clipping_planes;
   
      void MakeMainCamera();

      Matrix4 view_projection();

      void BeginRender();

      void EndRender();

      Camera() = default;
      Camera(Window* window) { this->window = window; } 
   private:
      Window* window;
   };

}
namespace Ignition::MainCamera {
   extern Camera* camera;
}
