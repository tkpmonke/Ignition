#include "gui/gui.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "gui/gizmos.hpp"

ImVec2 size;
bool click;

void Implosion::GUI::SceneView() {
   ImGuiWindowFlags flags = 0;//   ImGuiWindowFlags_NoScrollbar |
                              //ImGuiWindowFlags_NoScrollWithMouse;

   if (ImGui::Begin("Scene",nullptr,flags)) {
      size = ImGui::GetContentRegionAvail();
      ImVec2 contentPos = ImGui::GetCursorScreenPos();

      if (ImGui::IsWindowHovered()) {
         bool b = glfwGetMouseButton((GLFWwindow*)*this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

         if (b != click && b == true) { 
            this->RayCastMouse();
         }
         click = b;
      }

      ImGui::Image((void*)(intptr_t)this->color, size, ImVec2(0,0), ImVec2(1, -1));
      camera->size.x = size.x;
      camera->size.y = size.y;

      if (this->selectedObject != nullptr) {
         ImGuizmo::SetOrthographic(false);
         ImGuizmo::SetDrawlist();
         ImGuizmo::SetRect(contentPos.x, contentPos.y, size.x, size.y);

         Ignition::Matrix4 model = Ignition::Matrix4(1.f);
         model = glm::translate(model, this->selectedObject->transform.position);
         model *= glm::mat4_cast(glm::quat(glm::radians(this->selectedObject->transform.rotation)));
         model = glm::scale(model, this->selectedObject->transform.scale);
         

         if (ImGuizmo::Manipulate(glm::value_ptr(Ignition::mainCamera->ViewMatrix()),
                  glm::value_ptr(Ignition::mainCamera->ProjectionMatrix()),
                  Gizmos::currentGizmoOperation, ImGuizmo::WORLD,
                  glm::value_ptr(model))) {
             glm::vec3 translation, rotation, scale;
             ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model),
                   glm::value_ptr(translation),
                   glm::value_ptr(rotation),
                   glm::value_ptr(scale));
            
             this->selectedObject->transform.position = translation;
             this->selectedObject->transform.rotation = rotation;
             this->selectedObject->transform.scale = scale;
         }
      }
   }
   ImGui::End();
   
   ImGui::SetNextWindowSize(ImVec2(120, 80));

   if (ImGui::Begin("TransformModeSelection",nullptr, ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoTitleBar)) {
      if (ImGui::IsKeyPressed(ImGuiKey_W) 
            && !ImGui::IsKeyPressed(ImGuiKey_LeftCtrl)
            && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
         Gizmos::currentGizmoOperation = ImGuizmo::TRANSLATE;
      if (ImGui::IsKeyPressed(ImGuiKey_E)
            && !ImGui::IsKeyPressed(ImGuiKey_LeftCtrl)
            && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
         Gizmos::currentGizmoOperation = ImGuizmo::ROTATE;
      if (ImGui::IsKeyPressed(ImGuiKey_R)
            && !ImGui::IsKeyPressed(ImGuiKey_LeftCtrl) 
            && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
         Gizmos::currentGizmoOperation = ImGuizmo::SCALE;

      if (ImGui::RadioButton("Translate", Gizmos::currentGizmoOperation == ImGuizmo::TRANSLATE))
         Gizmos::currentGizmoOperation = ImGuizmo::TRANSLATE;
      if (ImGui::RadioButton("Rotate", Gizmos::currentGizmoOperation == ImGuizmo::ROTATE))
         Gizmos::currentGizmoOperation = ImGuizmo::ROTATE;
      if (ImGui::RadioButton("Scale", Gizmos::currentGizmoOperation == ImGuizmo::SCALE))
         Gizmos::currentGizmoOperation = ImGuizmo::SCALE;
   }
   ImGui::End();
}


