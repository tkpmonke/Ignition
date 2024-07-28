#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "object.hpp"
#include "camera.hpp"

namespace Implosion {
   class GUI {
   public:
      GUI(GLFWwindow*, Ignition::Camera*);
      
      void NewFrame();

      void Inspector(Ignition::Object*);
      
      void InitGrid();
      void RenderGrid();

      void FileExplorer();

      void SceneGraph(std::vector<Ignition::Object>*);

      void MenuBar(std::vector<Ignition::Object>*);

      void AddObjectMenu(std::vector<Ignition::Object>*);
      
      void DebugMenu();

      void Style();

      void EndFrame();

      void Shutdown();
   private:
      GLFWwindow* window;
      Ignition::Camera* camera;
   };
}
