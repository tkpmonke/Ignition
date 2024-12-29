#include "gui/gui.hpp"

namespace Implosion {
   void GUI::Style()
   {
      // Classic Steam style by metasprite from ImThemes
	// Modern style by LousyBook-01 from ImThemes
   	ImGuiStyle& style = ImGui::GetStyle();
   	
   	style.Alpha = 1.0f;
   	style.DisabledAlpha = 0.300000011920929f;
   	style.WindowPadding = ImVec2(10.10000038146973f, 10.10000038146973f);
   	style.WindowRounding = 10.30000019073486f;
   	style.WindowBorderSize = 1.0f;
   	style.WindowMinSize = ImVec2(20.0f, 32.0f);
   	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
   	style.WindowMenuButtonPosition = ImGuiDir_Right;
   	style.ChildRounding = 8.199999809265137f;
   	style.ChildBorderSize = 1.0f;
   	style.PopupRounding = 10.69999980926514f;
   	style.PopupBorderSize = 1.0f;
   	style.FramePadding = ImVec2(20.0f, 1.5f);
   	style.FrameRounding = 4.800000190734863f;
   	style.FrameBorderSize = 0.0f;
   	style.ItemSpacing = ImVec2(9.699999809265137f, 5.300000190734863f);
   	style.ItemInnerSpacing = ImVec2(5.400000095367432f, 9.300000190734863f);
   	style.CellPadding = ImVec2(7.900000095367432f, 2.0f);
   	style.IndentSpacing = 10.69999980926514f;
   	style.ColumnsMinSpacing = 6.0f;
   	style.ScrollbarSize = 12.10000038146973f;
   	style.ScrollbarRounding = 20.0f;
   	style.GrabMinSize = 10.0f;
   	style.GrabRounding = 4.599999904632568f;
   	style.TabRounding = 4.0f;
   	style.TabBorderSize = 0.0f;
   	style.TabMinWidthForCloseButton = 0.0f;
   	style.ColorButtonPosition = ImGuiDir_Right;
   	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
   	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
   	
   	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
   	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 0.3991416096687317f);
   	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 0.9399999976158142f);
   	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
   	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05098039284348488f, 0.05098039284348488f, 0.05098039284348488f, 0.9399999976158142f);
   	style.Colors[ImGuiCol_Border] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
   	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
   	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.4206008315086365f);
   	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1411764770746231f, 0.1411764770746231f, 0.1411764770746231f, 0.4000000059604645f);
   	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2313725501298904f, 0.2313725501298904f, 0.2313725501298904f, 0.8626609444618225f);
   	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
   	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.09411764889955521f, 0.09411764889955521f, 0.09411764889955521f, 1.0f);
   	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.2918455004692078f);
   	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
   	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
   	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
   	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
   	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
   	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9803921580314636f, 0.2588235437870026f, 0.2588235437870026f, 1.0f);
   	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
   	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9803921580314636f, 1.0f);
   	style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.0f, 0.0f, 0.5793991088867188f);
   	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
   	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.2313725501298904f, 0.2313725501298904f, 1.0f);
   	style.Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 0.454935610294342f);
   	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1803921610116959f, 0.1803921610116959f, 0.1803921610116959f, 0.800000011920929f);
   	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.9764705896377563f, 0.2588235437870026f, 0.2588235437870026f, 1.0f);
   	style.Colors[ImGuiCol_Separator] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
   	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
   	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
   	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.2000000029802322f);
   	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
   	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
   	style.Colors[ImGuiCol_Tab] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0f);
   	style.Colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.364705890417099f, 0.6745098233222961f, 0.800000011920929f);
   	style.Colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.2235294133424759f, 0.2235294133424759f, 1.0f);
   	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1098039224743843f, 0.168627455830574f, 0.239215686917305f, 0.9724000096321106f);
   	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
   	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
   	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
   	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
   	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.2156862765550613f, 0.6980392336845398f, 1.0f);
   	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(1.0f, 0.2352941185235977f, 0.2352941185235977f, 1.0f);
   	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0f, 0.3176470696926117f, 0.3176470696926117f, 1.0f);
   	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0f, 0.5647059082984924f, 0.5647059082984924f, 0.3690987229347229f);
   	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.7254902124404907f, 0.3372549116611481f, 1.0f, 0.0f);
   	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 0.2745098173618317f, 0.2745098173618317f, 0.1115880012512207f);
   	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9764705896377563f, 0.2588235437870026f, 0.2588235437870026f, 1.0f);
   	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
   	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 0.6437768340110779f);
   	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.4678111672401428f);
   	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.733905553817749f);
   	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.7982832789421082f);

   }
}
