#pragma once

#include "types/shader.hpp"
#include "window.hpp"
#include <vector>

namespace Ignition::Rendering {
   class PostProcess {
   public:
      PostProcess(std::string data, Window* window) : window(window), shader(data, ShaderType_Compute) {}
      PostProcess(std::string data, Window* window, int passes) : 
                  window(window), shader(data, ShaderType_Compute),
                  passes(passes) {}

      PostProcess(std::string data, Window* window, Vector2 kernalSize) : 
                  window(window), shader(data, ShaderType_Compute),
                  kernalSize(kernalSize) {}

      PostProcess(std::string data, Window* window, int passes, Vector2 kernalSize) : 
                  window(window), shader(data, ShaderType_Compute),
                  passes(passes), kernalSize(kernalSize) {}
      void Render();
         
      void MakeExtraTexture();

      int passes = 1;
      Vector2 kernalSize = Vector2(10,10);
      PostProcess() = default;
   private:
      Window* window;
      Shader shader;
      std::vector<uint> extraTextures;
   };
}
