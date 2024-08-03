#include "window.hpp"
#include "stdio.h"
#include "input/commands.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
   if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
      return;

  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

namespace Ignition {
   Window::Window(int w, int h, const char* c)
   {
      glfwInit();
      glfwWindowHint(GLFW_FLOATING, GL_TRUE);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
      glfwWindowHint(GLFW_SAMPLES, 8);
      this->window = glfwCreateWindow(w,h,c,NULL,NULL);
      glfwMakeContextCurrent(this->window);
      glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
      glewInit();
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS); 
      glEnable(GL_BLEND);  
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_MULTISAMPLE); 
#ifdef DEBUG
      glEnable(GL_DEBUG_OUTPUT);
      glDebugMessageCallback(MessageCallback, 0);
#endif
      SetKeyCallback(this->window);
   }

   bool Window::IsOpen() { return !glfwWindowShouldClose(this->window); }

   void Window::Update() { glfwPollEvents(); }

   void Window::Shutdown()
   {
      glfwDestroyWindow(this->window);
      glfwTerminate();
   }
}
