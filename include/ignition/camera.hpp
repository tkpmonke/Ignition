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
      } clippingPlanes;

      enum {
         Perspective = 0,
         Orthographic = 1
      } projectionMode = Perspective;
   
      void MakeMainCamera();

      Matrix4 ViewProjectionMatrix();

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
   
   extern Camera* mainCamera;
}
