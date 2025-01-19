#include "types/texture.hpp"
#include "utils/files.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

int i = 0;
namespace Ignition::Rendering {
   std::unordered_map<std::string, int> texture_lookup_table;

   void Texture::LoadFlags() {

      if (this->sampler == 0)
         glGenSamplers(1, &this->sampler);

      glBindSampler(0, this->sampler);
      TextureFlags f = static_cast<TextureFlags>(flags);

      if ((f & TextureFlags_Repeat)) {
         glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_REPEAT);
      }
      if ((f & TextureFlags_Clamp)) {
         glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      }
      if ((f & TextureFlags_Mirrored_Repeat)) {
         glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
         glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
         glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
      }
      
      
      if ((f & TextureFlags_Nearest)) {
         glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
      if ((f & TextureFlags_Linear)) {
         glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }


   }

   void Texture::LoadData(std::string file)
   {
      int w, h, nr;
      stbi_set_flip_vertically_on_load(true);
      unsigned char* d = stbi_load((Ignition::IO::GetProjectHome()+file).data(), &w, &h, &nr, 0);
      LoadData(d, w, h, nr, file); 
   }

   void Texture::LoadData(std::vector<std::string> files) {
      this->name = files[0];

      if (location == 0) 
         glGenTextures(1, &this->location);

      glBindTexture(GL_TEXTURE_CUBE_MAP, this->location);
      for (int i = 0; i < files.size(); ++i) {
         int w, h, nr;
         stbi_set_flip_vertically_on_load(false);
         unsigned char* d = stbi_load(files[i].data(), &w, &h, &nr, 0);
         LoadData(d, w, h, nr, files[i], i+1);
      }
   }

   void Texture::LoadData(std::vector<const char*> data, int w, int h, int nr, std::string name) {
      this->name = name;

      if (location == 0) 
         glGenTextures(1, &this->location);

      glBindTexture(GL_TEXTURE_CUBE_MAP, this->location);
      for (int i = 0; i < data.size(); ++i) {
         LoadData((unsigned char*)data[i], w, h, nr, name, i+1);
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
            LoadFlags();
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

      glGenSamplers(1, &this->sampler);
      LoadFlags();
      GLenum format;
      switch (nr) {
         case(1):
            format = GL_RED;
            break;
         case(2):
            format = GL_RG;
            break;
         default:
         case(3):
            format = GL_RGB;
            break;
         case(4):
            format = GL_RGBA;
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
