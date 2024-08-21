#pragma once

#include "types/shader.hpp"
#include "window.hpp"

namespace Ignition::Rendering {
   class PostProcess {
   public:
      PostProcess(std::string data, Window* window) : window(window), shader(data, ShaderType::Compute) {}
      void Render();
      PostProcess() = default;
   private:
      Window* window;
      Shader shader;
   };
}
