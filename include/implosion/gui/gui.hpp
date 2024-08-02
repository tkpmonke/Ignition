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
      void RemakeGrid();

      void FileExplorer();

      void SceneGraph(std::vector<Ignition::Object>*);

      void MenuBar(std::vector<Ignition::Object>*);

      void AddObjectMenu(std::vector<Ignition::Object>*);
      
      void DebugMenu();

      void Preferences();

      void Style();

      void EndFrame();

      void Shutdown();

      float gridSpacing = 1.f, gridSize = 10000, gridHeight = 0;
      Ignition::Vector4 gridColor = Ignition::Vector4(.4f, .4f, .4f, 5.f);
   private:
      GLFWwindow* window;
      Ignition::Camera* camera;
      uint gridVao, gridVbo, gridProgram, gridVertSize;
   };
}
