#include "window.hpp"

namespace Overload {
   Window::Window(int w, int h, const char* c)
   {
      glfwInit();
      this->window = glfwCreateWindow(w,h,c,NULL,NULL);
      glfwMakeContextCurrent(this->window);

      glewInit();
      glEnable(GL_DEPTH_TEST);
   }

   bool Window::IsOpen() { return !glfwWindowShouldClose(this->window); }

   void Window::Update() { glfwPollEvents(); }

   void Window::Shutdown()
   {
      glfwDestroyWindow(this->window);
      glfwTerminate();
   }
}
