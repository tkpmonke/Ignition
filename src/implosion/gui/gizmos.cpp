#include "gui/gizmos.hpp"


#include <GL/glew.h>
#include <string>

unsigned int vao;
unsigned int program;


namespace Implosion::Gizmos {

   ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
   ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);
   

   void InitilizeGizmos() {
      ImGuizmo::BeginFrame();
   }
}

