#pragma once
#include "types/transform.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "imguizmo/ImGuizmo.h" 


namespace Implosion::Gizmos {

   extern ImGuizmo::OPERATION currentGizmoOperation;
   extern ImGuizmo::MODE currentGizmoMode;
   
   void InitilizeGizmos();
   void RenderGizmo(Ignition::Vector3 position);
}
