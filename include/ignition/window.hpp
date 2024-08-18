#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "types/shader.hpp"

namespace Ignition {
   class Window {
   public:
      Window() = default;
      Window(int, int, const char*);
      
      bool IsOpen();

      void Update();
      
      void Shutdown();

      void Bind();

      void Resize(int w, int h);

      operator GLFWwindow*() const { return this->window; };

      unsigned int framebuffer, color, depth;
      
      friend class Camera;
   private:
      GLFWwindow* window;
      unsigned int vao;
      Rendering::Shader s;
      void FramebufferCallback(GLFWwindow*, int, int);
   };
}
