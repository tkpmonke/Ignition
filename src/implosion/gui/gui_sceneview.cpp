#include "gui/gui.hpp"
#include <iostream>

ImVec2 size;
bool click;

void Implosion::GUI::SceneView() {
   ImGuiWindowFlags flags =   ImGuiWindowFlags_NoScrollbar |
                              ImGuiWindowFlags_NoScrollWithMouse;

   if (ImGui::Begin("Scene",nullptr,flags)) {
      size = ImGui::GetWindowSize();
      ImGui::Image((void*)(intptr_t)this->color, size, ImVec2(0,0), ImVec2(1, -1));
      camera->size.x = size.x;
      camera->size.y = size.y;

      if (ImGui::IsWindowHovered()) {
         bool b = glfwGetMouseButton((GLFWwindow*)*this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

         if (b != click && b == true) { 
            this->RayCastMouse();
         }
         click = b;
      }
   }
   ImGui::End();
   
}


