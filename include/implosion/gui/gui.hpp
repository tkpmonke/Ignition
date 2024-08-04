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

      void SceneGraph();

      void MenuBar();

      void AddObjectMenu();
      
      void DebugMenu();

      void Preferences();

      void Style();

      void EndFrame();

      void Shutdown();

      float gridSpacing = 1.f, gridSize = 10000, gridHeight = 0, gridDistance = 30, gridFalloff = 10;
      Ignition::Vector4 gridColor = Ignition::Vector4(.4f, .4f, .4f, .5f);

      float gridWidth = 1;

      bool enableGrid = true, enableDistanceFalloff = true;
      bool vsync = true;
      bool anti = true;
      GLFWwindow* window;
      Ignition::Camera* camera;
      uint gridVao, gridVbo, gridProgram, gridVertSize;

      GUI() = default;
   };
}
