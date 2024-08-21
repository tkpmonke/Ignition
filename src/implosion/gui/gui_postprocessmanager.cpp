#include "gui/gui.hpp"

using namespace Ignition::Rendering;

namespace Implosion {
static int selected_item = -1;
void GUI::PostProcessManagerUI() {
  ImGui::Begin("Post Processing");

  if (ImGui::BeginChild("Order", ImVec2(300, 500))) {
    for (int i = 0; i < pp_manager.effects.size(); ++i) {
      bool s = (selected_item == i);
      if (ImGui::Selectable(pp_manager.effects[pp_manager.index[i]].name.data(), s)) {
        selected_item = i;
      }
      ImGui::Checkbox(("##pp_enabled"+pp_manager.effects[i].name).data(), &pp_manager.effects[pp_manager.index[i]].enabled); 
    }

    if (selected_item != -1) {
      ImGui::Text(
          "Selected: %s",
          pp_manager.effects[pp_manager.index[selected_item]].name.data());
    }
    ImGui::EndChild();
  }
  ImGui::SameLine();
  if (ImGui::BeginChild("Options")) {
    ImGui::BeginDisabled(selected_item < 1);
    if (ImGui::Button("Move Up")) {
      std::swap(pp_manager.index[selected_item],
                pp_manager.index[selected_item - 1]);
      selected_item--;
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(selected_item > 3 || selected_item == -1);
    if (ImGui::Button("Move Down")) {
      std::swap(pp_manager.index[selected_item],
                pp_manager.index[selected_item + 1]);
      selected_item++;
    }
    ImGui::EndDisabled();
    ImGui::EndChild();
  }

  ImGui::End();

  pp_manager.RenderEffects();
}
} // namespace Implosion
