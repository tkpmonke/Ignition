#pragma once

#include "GL/glew.h"
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/io.hpp"

#define IGNITION_CUBE_MAP     4
#define IGNITION_2D           2

#define IGNITION_RGBA32F      GL_RGBA32F
#define IGNITION_RGBA         GL_RGBA 
#define IGNITION_RGB          GL_RGB
#define IGNITION_RG           GL_RG
#define IGNITION_R            GL_RED
#define IGNITION_DEPTH        GL_DEPTH_COMPONENT

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

      /// Load data from file
      void LoadData(std::string file);

      /// Load cube map from multiple files
      void LoadData(std::vector<std::string> file);

      /// Load cube map from char[]
      void LoadData(std::vector<const char*> data, int w, int h, int nr, std::string name);
      /// Load data from unsigned char[]
      void LoadData(unsigned char* data, int w, int h, int nr, std::string name);

      /// location of the texture (used by opengl)
      unsigned int location = 0;

      /// name of texture / path of texture
      std::string name;
      operator int() const { return location; };
      int type = IGNITION_2D;
   private:
      int flags;
      void LoadData(unsigned char* data, int w, int h, int nr, std::string name, int i);
   };

   class RenderTexture {
   public:
      RenderTexture() = default;
      unsigned int framebuffer, color, depth;

      unsigned int width, height, format;

      void Bind() {
         glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
         glViewport(0, 0, width, height);
      }

      void Resize(int x, int y) {
         width = x; height = y;
         Bind();
      }

      RenderTexture(int width, int height, int format) : width(width), height(height), format(format) {
         glGenFramebuffers(1, &framebuffer);
         glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

         if (format != IGNITION_DEPTH) {
            glGenTextures(1, &color);
            glBindTexture(GL_TEXTURE_2D, color);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

            glGenTextures(1, &depth);
            glBindTexture(GL_TEXTURE_2D, depth);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
         } else {
            glGenTextures(1, &depth);
            glBindTexture(GL_TEXTURE_2D, depth);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
         }


         if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	         Ignition::IO::FatalError("Framebuffer is not complete!");

         glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }
      
   };
}
