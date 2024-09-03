#include "gui/gui.hpp"
#include "utils/files.hpp"

#include <iostream>

namespace Implosion {
   std::string s = FS::GetHome() + "/Implosion/gui";
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
      glBindFramebuffer(GL_FRAMEBUFFER, window->framebuffer);
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
