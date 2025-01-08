#pragma once
#include "window.hpp"
#include "object.hpp"

#include <glm/mat4x4.hpp>

namespace Ignition {
   class Camera : public Object {
   public:
      /**
       * Value of current FOV 
       */
      float fov;

      
      /// Struct containing the min and max values, used by projection matrix
      struct ClippingPlanes {
         float min, max;
      } clippingPlanes;

      /// Enum containing information on weither to render with perspective or not, used by projection matrix
      enum ProjectionMode {
         Perspective = 0,
         Orthographic = 1
      } projectionMode = Perspective;
   
      /// Sets Main Camera To This
      void MakeMainCamera();

      Matrix4 ViewProjectionMatrix();

      Matrix4 ViewMatrix();
      Matrix4 ProjectionMatrix();

      void BeginRender();

      void EndRender(bool);

      void EndGUI();

      Camera() = default;

      /// prefered constructor
      Camera(Window* window) { this->window = window; } 

      /// INTERNAL USE ONLY
      Matrix4 i_viewProj;
      /// INTERNAL USE ONLY
      Matrix4 i_lightProj;

      Window* window;

      /// Size of current Framebuffer (or window if framebuffer is 0)
      Vector2 size;
   };
   
   extern Camera* mainCamera;
}
