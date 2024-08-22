#include "utils/postprocess.hpp"
#include "GL/glew.h"

namespace Ignition::Rendering {
   void PostProcess::Render() {
      
      glUseProgram(this->shader.program);
      glBindImageTexture(0, this->window->color, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

      for (int i = 1; i <= extraTextures.size(); ++i)
         glBindImageTexture(i, extraTextures[i-1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
      
      int w,h;
      glfwGetFramebufferSize((GLFWwindow*)*this->window, &w, &h);
      
      for (int i = 0; i < passes; ++i)
      {
         glUniform1i(glGetUniformLocation(this->shader.program, "passID"), i);
         glDispatchCompute((unsigned int)(w/kernalSize.x), (unsigned int)(h/kernalSize.y), 1);
         glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
      }
   }

   void PostProcess::MakeExtraTexture() {
      uint tex;
      int width, height;
      glfwGetFramebufferSize((GLFWwindow*)*window, &width, &height);
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      extraTextures.push_back(tex);
   }
}
