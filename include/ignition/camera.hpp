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

      Matrix4 ViewMatrix();
      Matrix4 ProjectionMatrix();

      void BeginRender();

      void EndRender(bool);

      void EndGUI();

      Camera() = default;
      Camera(Window* window) { this->window = window; } 
      Matrix4 viewProj;

      Window* window;

      Vector2 size;
   };

}
namespace Ignition::MainCamera {
   extern Camera* camera;
}
