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

   ImVec2 size;
   
   void GUI::EndFrame()
   {
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glBindFramebuffer(GL_FRAMEBUFFER, window->framebuffer);
      window->Resize(size.x, size.y);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }
   
   void GUI::SceneView() {
      ImGuiWindowFlags flags =   ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_NoScrollWithMouse;

      if (ImGui::Begin("Scene",nullptr,flags)) {
         size = ImGui::GetWindowSize();

         
         
         ImGui::Image((void*)(intptr_t)this->color, size, ImVec2(0,0), ImVec2(1, -1));
         camera->size.x = size.x;
         camera->size.y = size.y;

         

      }
      ImGui::End();
   }

   void GUI::Shutdown()
   {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
   }
}
