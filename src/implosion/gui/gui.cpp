#include "gui/gui.hpp"
#include "utils/files.hpp"
#include "input/commands.hpp"

#include <iostream>

#include "sprites/file.hpp"
#include "sprites/folder.hpp"
#include "sprites/empty_folder.hpp"
#include "sprites/igscn.hpp"
#include "sprites/lua_script.hpp"

namespace Implosion {
   std::string s = Ignition::IO::GetHome() + "/Implosion/gui";
   GUI::GUI(Ignition::Window* window, Ignition::Camera* camera)
   {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::SetCurrentContext(ImGui::GetCurrentContext());
      ImGuiIO& io = ImGui::GetIO();
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
      io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
      io.IniFilename = s.data(); 

      this->window = window;
      this->camera = camera;
      color = window->color;

      SetCommandCallback((GLFWwindow*)*window, this);
      ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)*this->window, true);
      ImGui_ImplOpenGL3_Init("#version 330");

      this->files.folder.SetFlags(Ignition::Rendering::Linear);
      this->files.folder.LoadData(        FOLDER_IMAGE_PIXEL_DATA,
                                          FOLDER_IMAGE_WIDTH,
                                          FOLDER_IMAGE_HEIGHT,
                                          FOLDER_IMAGE_BYTES_PER_PIXEL,
                                          "Implosion_Folder");

      this->files.empty_folder.SetFlags(Ignition::Rendering::Linear);
      this->files.empty_folder.LoadData(  EMPTY_FOLDER_IMAGE_PIXEL_DATA,
                                          EMPTY_FOLDER_IMAGE_WIDTH,
                                          EMPTY_FOLDER_IMAGE_HEIGHT,
                                          EMPTY_FOLDER_IMAGE_BYTES_PER_PIXEL,
                                          "Implosion_Empty_Folder");

      this->files.file.SetFlags(Ignition::Rendering::Linear);
      this->files.file.LoadData(          FILE_IMAGE_PIXEL_DATA,
                                          FILE_IMAGE_WIDTH,
                                          FILE_IMAGE_HEIGHT,
                                          FILE_IMAGE_BYTES_PER_PIXEL,
                                          "Implosion_File");

      this->files.additionalTextures.igscn.SetFlags(Ignition::Rendering::Linear);
      this->files.additionalTextures.igscn.LoadData(  igscn_PIXEL_DATA,
                                                      igscn_WIDTH,
                                                      igscn_HEIGHT,
                                                      igscn_BYTES_PER_PIXEL,
                                                      "Implosion_IGSCN");
      
      this->files.additionalTextures.lua_script.SetFlags(Ignition::Rendering::Linear);
      this->files.additionalTextures.lua_script.LoadData(  LUA_SCRIPT_PIXEL_DATA,
                                                      LUA_SCRIPT_WIDTH,
                                                      LUA_SCRIPT_HEIGHT,
                                                      LUA_SCRIPT_BYTES_PER_PIXEL,
                                                      "Implosion_LUA_SCRIPT");
   }

   void GUI::NewFrame()
   {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
   
      ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
   }

   
   void GUI::EndFrame()
   {
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      window->Resize(camera->size.x, camera->size.y);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

   void GUI::Shutdown()
   {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
   }
}
