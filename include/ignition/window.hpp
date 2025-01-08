#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "types/shader.hpp"

namespace Ignition {

   /**
    * Handles windowing
    */

   class Window {
   public:
      Window() = default;
      Window(int width, int height, const char* name, bool* isOpen);
      
      /// Is The Window Open
      bool IsOpen();

      /// Update window
      void Update();
      
      /// Called on Shutdown
      void Shutdown();

      /// Bind Window for rendering
      void Bind();

      /// Resize Framebuffer
      void Resize(int w, int h);

      /// Restart Application
      void Restart();

      /// Close Application
      void Close();

      operator GLFWwindow*() const { return this->window; };

      /// INTERNAL ONLY OR IF YOU KNOW WHAT YOUR DOING
      unsigned int framebuffer, color, depth;
      
      friend class Camera;
   private:
      GLFWwindow* window;
      unsigned int vao;
      Rendering::Shader s;
      bool* isopen;
   };
}
