#include "window.hpp"
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
namespace Ignition {
   Window::Window(int w, int h, const char* c)
   {
      glfwInit();
      glfwWindowHint(GLFW_FLOATING, GL_TRUE);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
      this->window = glfwCreateWindow(w,h,c,NULL,NULL);
      glfwMakeContextCurrent(this->window);
      glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
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
