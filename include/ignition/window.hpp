#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Ignition {
   class Window {
   public:
      Window() = default;
      Window(int, int, const char*);
      
      bool IsOpen();

      void Update();
      
      void Shutdown();

      operator GLFWwindow*() const { return this->window; };

   private:
      GLFWwindow* window;
   };
}
