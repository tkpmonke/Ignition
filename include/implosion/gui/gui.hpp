#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "object.hpp"
#include "camera.hpp"

#include <optional>

#include "gui/popup.hpp"
#include "utils/postprocessingmanager.hpp"

namespace Implosion {
   class GUI {
   public:
      GUI(Ignition::Window*, Ignition::Camera*);
      
      void NewFrame();

      void Inspector();
      
      void InitGrid();
      void RenderGrid();
      void RemakeGrid();

      void FileExplorer();
      void RefreshFiles();

      void SceneHierarchy();

      void MenuBar();

      void AddObjectMenu();
      
      void DebugMenu();

      void Preferences();

      void Style();

      void SceneView();

      void RenderPopups();

      void InitAssetLoader();

      void RayCastMouse();

      void PostProcessManagerUI();

      void EndFrame();

      void Shutdown();

      float gridSpacing = 1.f, gridSize = 10000, gridHeight = 0, gridDistance = 30, gridFalloff = 10;
      Ignition::Vector4 gridColor = Ignition::Vector4(.4f, .4f, .4f, .5f);

      float gridWidth = 1;

      bool enableGrid = true, enableDistanceFalloff = true;
      bool vsync = true;
      bool anti = true;

      Ignition::Window* window;
      
      int color;
      
      Ignition::Camera* camera;
      
      uint gridVao, gridVbo, gridProgram, gridVertSize, gridXYVao, gridXYVbo, gridXYProgram;
      
      Ignition::Object* selectedObject = nullptr;
      std::optional<Ignition::Object> copiedObject;

      GUI() = default;

      std::vector<Implosion::Popup> popups;

#ifdef __linux__
      uint coreCount = 0;
      float cpuSpeed = 0;
      std::string cpuModel = "";
#endif 
   
      struct {
         struct FileSpec {std::string name,path; int type;};
         std::vector<FileSpec> files;
         std::string activeDirectory;
         float size = 50;

         Ignition::Rendering::Texture folder, file, empty_folder;

         struct {
            Ignition::Rendering::Texture  igscn,
                                          lua_script,
                                          angelscript_script;
         } additionalTextures;
      } files;
   private:
      bool openAssetLoader = false;
      Ignition::Rendering::Texture tex;
   };
}
