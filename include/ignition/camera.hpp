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

      /// Returns ProjectionMatrix * ViewMatrix
      Matrix4 ViewProjectionMatrix();

      /// Returns View Matrix
      Matrix4 ViewMatrix();

      /// Returns Projection Matrix
      Matrix4 ProjectionMatrix();

      /// Call at beginning of rendering
      void BeginRender();

      /// End Rendering, pass true if a gui needs to render after the rest of the scene
      void EndRender(bool hasGUI);

      /// If EndRender was told that a gui was rendering, use this to finish rendering the gui
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
