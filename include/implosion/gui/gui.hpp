#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "object.hpp"

namespace Implosion {
   class GUI {
   public:
      GUI();
      
      void NewFrame();

      void Inspector(Ignition::Object);
      
      void InitGrid();
      void RenderGrid();

      void FileExplorer();

      void SceneGraph(std::vector<Ignition::Object>);

      void MenuBar();

      Ignition::Object AddObject();
      
      void DebugMenu();

      void EndFrame();

      void Shutdown();
   };
}
