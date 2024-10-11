#pragma once

#include "scene.hpp"
#include "window.hpp"

#include <vector>

namespace Ignition {
   class Project {
   public:
      std::vector<Scene> scenes;
      int currentScene;
      std::string name;

      Project() = default;
      void LoadProject(Window* win);
   };
   extern Project project; 
}
