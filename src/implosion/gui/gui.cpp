#include "gui/gui.hpp"
#include "utils/files.hpp"

#include <iostream>

#include "sprites/file.hpp"
#include "sprites/folder.hpp"
#include "sprites/empty_folder.hpp"

namespace Implosion {
   std::string s = Ignition::IO::GetHome() + "/Implosion/gui";
   GUI::GUI(Ignition::Window* window, Ignition::Camera* camera)
   {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::SetCurrentContext(ImGui::GetCurrentContext());
      ImGuiIO& io = ImGui::GetIO();
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
      io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
      io.IniFilename = s.data(); 

      this->window = window;
      this->camera = camera;
      color = window->color;

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
