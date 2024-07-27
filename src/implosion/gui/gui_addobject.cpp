#include "gui/gui.hpp"
#include "components/rendering/meshrenderer.hpp"
#include "GLFW/glfw3.h"
#include "shapes/square.hpp"
#include <memory>

using namespace Ignition::Rendering;

namespace Implosion {
   Ignition::Object GUI::AddObjectMenu()
   {
      Ignition::Object o = Ignition::Object();
      
      bool open = true;

      ImGui::OpenPopup("Add Object Menu");
      
      while (open) {
         glfwPollEvents();
         NewFrame();


         if (ImGui::BeginPopupModal("Add Object Menu"))
         {
            ImGui::Text("do stuff");
            
            if (ImGui::BeginMenuBar())
            {
               if (ImGui::MenuItem("Exit"))
               {
                  open = false;
                  ImGui::CloseCurrentPopup();
               }
            }

            if (ImGui::Button("Empty"))
            {
               return o;
            }
            ImGui::SameLine();
            if (ImGui::Button("Square"))
            {
               MeshRenderer m = MeshRenderer();
               m.LoadModel(square_model);
               o.AddComponent(std::shared_ptr(m));
            }

            ImGui::EndPopup();
         }

         EndFrame();
         glfwSwapBuffers(this->window);
      }
      return o;
   }
}
