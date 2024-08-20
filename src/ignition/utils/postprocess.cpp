#include "utils/postprocess.hpp"
#include "GL/glew.h"

namespace Ignition::Rendering {
   void PostProcess::Render() {
      
      glUseProgram(this->shader.program);
      glBindImageTexture(0, this->window->color, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
      
      int w,h;
      glfwGetFramebufferSize((GLFWwindow*)*this->window, &w, &h);

      glDispatchCompute((unsigned int)w/10, (unsigned int)h/10, 1);

      glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
   }
}
