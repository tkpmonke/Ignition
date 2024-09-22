#include "types/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

#include <iostream>

int i = 0;
namespace Ignition::Rendering {
   std::unordered_map<std::string, int> texture_lookup_table;
   void Texture::LoadData(std::string file)
   {
      int w, h, nr;
      stbi_set_flip_vertically_on_load(true);
      unsigned char* d = stbi_load(file.data(), &w, &h, &nr, 0);
      LoadData(d, w, h, nr, file); 
   }

   void Texture::LoadData(std::vector<std::string> files) {
         glGenTextures(1, &this->location);
         glBindTexture(GL_TEXTURE_CUBE_MAP, this->location);
      for (int i = 0; i < files.size(); ++i) {
         int w, h, nr;
         stbi_set_flip_vertically_on_load(false);
         unsigned char* d = stbi_load(files[i].data(), &w, &h, &nr, 0);
         LoadData(d, w, h, nr, files[i], i+1);
      }
   }

   void Texture::LoadData(unsigned char* data, int w, int h, int nr, std::string name)
   {
      this->name = name;
      for (auto [key, value] : texture_lookup_table)
      {
         if (name == key)
         {
            this->location = value;
            return;
         }
      }


      LoadData(data, w, h, nr, name, 0);
         
      texture_lookup_table[name] = this->location;
       
   }
   
   void Texture::LoadData(unsigned char* data, int w, int h, int nr, std::string name, int i) {

      GLenum texType;
      switch (this->type) {
         case(IGNITION_2D):
            texType = GL_TEXTURE_2D;
            break;
         case(IGNITION_CUBE_MAP):
            texType = GL_TEXTURE_CUBE_MAP;
            break;
      }

      if (i == 0) {
         glGenTextures(1, &this->location);
         glBindTexture(texType, this->location);
      }
      if (flags & TextureFlags::Repeat) {
         glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glTexParameteri(texType, GL_TEXTURE_WRAP_R, GL_REPEAT);
      }
      if (flags & TextureFlags::Clamp) {
         glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         glTexParameteri(texType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      }
      if (flags & TextureFlags::Mirrored_Repeat) {
         glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
         glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
         glTexParameteri(texType, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
      }
      
      if (flags & TextureFlags::Linear) {
         glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
      if (flags & TextureFlags::Nearest) {
         glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }

      GLenum format;
      switch (nr) {
         case(1):
            format = GL_RED;
            break;
         case(2):
            format = GL_RG;
            break;
         case(3):
            format = GL_RGB;
            break;
         case(4):
            format = GL_RGBA;
            break;
         default:
            format = GL_RGB;
            break;
      }
      if (i != 0) {
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+(i-1), 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
      } else {
         glTexImage2D(texType, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
         glGenerateMipmap(texType);
      }
   }
}
