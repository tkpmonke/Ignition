#pragma once

#include "GL/glew.h"
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/io.hpp"

#define IGNITION_CUBE_MAP     4
#define IGNITION_2D           2

namespace Ignition::Rendering {

   extern std::unordered_map<std::string, int> texture_lookup_table;

   enum TextureFlags {
      Repeat,
      Clamp,
      Mirrored_Repeat,
      
      Nearest,
      Linear
   };

   class Texture {
   public:
      Texture() = default;

      void SetFlags(int flags) { this->flags = flags; }
      void SetType(int type) { this->type = type; }
      void LoadData(std::string file);
      void LoadData(std::vector<std::string> file);
      void LoadData(std::vector<const char*> data, int w, int h, int nr, std::string name);
      void LoadData(unsigned char* data, int w, int h, int nr, std::string name);
      unsigned int location = 0;
      std::string name;
      operator int() const { return location; };
      int type = IGNITION_2D;
   private:
      int flags;
      void LoadData(unsigned char* data, int w, int h, int nr, std::string name, int i);
   };

   class RenderTexture {
   public:
      unsigned int framebuffer, color, depth;

      unsigned int width, height;

      void Bind() {
         glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      }

      void Resize(int x, int y);

      RenderTexture(int width, int height) : width(width), height(height) {
         glGenFramebuffers(1, &framebuffer);
         glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

         glGenTextures(1, &color);
         glBindTexture(GL_TEXTURE_2D, color);
         glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_RGBA32F, width, height, GL_TRUE);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, color, 0);

         glGenTextures(1, &depth);
         glBindTexture(GL_TEXTURE_2D, depth);

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

         glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

         if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	         Ignition::IO::FatalError("Framebuffer is not complete!");

         glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }
      
   };
}
