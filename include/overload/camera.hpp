#pragma once
#include "object.hpp"
#include "window.hpp"

#include <glm/mat4x4.hpp>

namespace Overload {
   class Camera : public Object {
   public:
      float fov;
      struct {
         float min, max;
      } clipping_planes;
   
      // Overload::MainCamera::camera = this;
      void MakeMainCamera();

      Matrix4 view_projection();

      void BeginRender();

      void EndRender();

      Camera() = default;
      Camera(Window* window) : window(window) {} 
   private:
      Window* window;
   };

   namespace Overload::MainCamera {
      extern Camera* camera;
   }
}
