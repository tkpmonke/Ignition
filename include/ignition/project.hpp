#pragma once

#include "scene.hpp"

#include <vector>

namespace Ignition {
   class Project {
   public:
      std::vector<Scene> scenes;
      int currentScene;
      std::string name;

      Project(int);
      Project() = default;
   };
   extern Project project; 
}
