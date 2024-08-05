#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void Style();

bool isInstalled = true;

bool checkDependinces()
{
   if (std::system("curl --help") != 0)
   {
#ifdef __linux__
      std::system("clear");
#endif 
      std::cerr << "Curl needs to be installed to use the hub\n";
      return 0;
   }
   if (std::system("python --version") != 0)
   {
#ifdef __linux__
      std::system("clear");
#endif 
      std::cerr << "Python needs to be installed to use the hub\n";
      return 0;
   }

#ifdef __linux__
      std::system("clear");
#endif 
   return 1;
}

struct Release {
   enum {
      Alpha,
      Beta,
      Released
   } stage;
   int major, minor, patch;
};

std::vector<Release> releases;
std::vector<std::string> releaseStrs;

std::string releasetostr(Release* r)
{
   std::string s;
   s += std::to_string(r->major) + '.' + std::to_string(r->minor) + '.' + std::to_string(r->patch);
   switch(r->stage)
   {
      case(Release::Alpha):
         s += "-alpha";
         break;
      case(Release::Beta):
         s += "-beta";
         break;
      case(Release::Released):
         break;
   }
   return s;
}


int main()
{
   glfwInit();
   glfwWindowHint(GLFW_FLOATING, true);
   glfwWindowHint(GLFW_RESIZABLE, false);
   GLFWwindow* window = glfwCreateWindow(640, 480, "Implosion Hub", NULL, NULL); 
   glfwMakeContextCurrent(window);

   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGui::SetCurrentContext(ImGui::GetCurrentContext());
   ImGuiIO& io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 330");

   Style();

   if (!checkDependinces())
      return -1;

   if (!std::system("implosion -i"))
   {
      if (getuid()) 
      {
         std::cerr << "Superuser Privlege is required for installer\n";
         return -1;
      }

      FILE* scraper = popen("python scraper.py", "r");
      char* scraped = (char*)malloc(64 * sizeof(char));
      while (fgets(scraped, 64, scraper) != NULL)
      {
         std::string s = scraped;
         Release r;

         int majorId = s.find('.', 1)-1;
         int minorId = s.find('.', majorId)+2;
         int patchId = s.find('-', minorId);


         r.major = std::stoi(s.substr(1, majorId));
         r.minor = std::stoi(s.substr(majorId+2, minorId-1));
         r.patch = std::stoi(s.substr(minorId+1, patchId-1));
         if (s.size() > patchId+1) {
            switch(scraped[patchId+1])
            {
               case('b'):
                  r.stage = Release::Beta;
                  break;
               case('a'):
                  r.stage = Release::Alpha;
                  break;
               default:
                  r.stage = Release::Beta;
            }
         } else {
            r.stage = Release::Released;
         }

         std::string st = releasetostr(&r);
         std::cout << st << "\n";
         releaseStrs.push_back(st);
      }
      
      isInstalled = false;
   }
   ImGuiWindowFlags flags = 
                 ImGuiWindowFlags_NoMove 
               | ImGuiWindowFlags_NoResize 
               | ImGuiWindowFlags_NoCollapse 
               | ImGuiWindowFlags_NoSavedSettings 
               | ImGuiWindowFlags_NoTitleBar;

   int g = -1;
   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      if (!isInstalled)
      {

         ImGui::Begin("Installer", NULL, flags);
         {

            ImGui::SetWindowPos(ImVec2(0, 0));
	         ImGui::SetWindowSize(ImVec2(640, 480));
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(50, 25.f));
            ImGui::SetNextItemWidth(625);
            if (ImGui::BeginCombo("##Version", g == -1 ? "Version" : releaseStrs[g].data()))
            {
               for (int i = 0; i < releaseStrs.size(); ++i)
               {
                  bool selected = (i == g);

                  if (ImGui::Selectable(releaseStrs[i].data(), selected))
                  {
                     g = i;
                  }

               }
               ImGui::EndCombo();
            }

            if (ImGui::Button("Download", ImVec2(625, 0)))
            {

            }


            ImGui::PopStyleVar(1);

            ImGui::End(); 
         }
      }
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glfwSwapBuffers(window);
   }

   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();
   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}

void Style()
   {
      ImGuiStyle style = ImGui::GetStyle();
      style.FrameRounding = 3;
      style.WindowPadding = ImVec2(20, 8);
      style.FramePadding = ImVec2(4, 3);
      style.ItemSpacing = ImVec2(9, 2);
      style.ItemInnerSpacing = ImVec2(5, 4);
      style.IndentSpacing = 4;
      style.ScrollbarSize = 12;
      style.ScrollbarRounding = 12;
      style.WindowBorderSize = 0;
      style.ChildBorderSize = 1;
      style.PopupBorderSize = 1;
      style.FrameBorderSize = 0;
      style.TabBorderSize = 0;
      style.TabBarBorderSize = 1;
      style.WindowRounding = 8;
      style.ChildRounding = 3;
      style.PopupRounding = 8;
      style.ScrollbarRounding = 9;
      style.CellPadding = ImVec2(5,3);
      style.SeparatorTextAlign = ImVec2(0, .5f);
      style.DockingSeparatorSize = 2;

      ImVec4* colors = ImGui::GetStyle().Colors;
      colors[ImGuiCol_Text]                   = ImVec4(0.00f, 1.00f, 0.85f, 0.77f);
      colors[ImGuiCol_TextDisabled]           = ImVec4(0.09f, 0.50f, 0.54f, 1.00f);
      colors[ImGuiCol_WindowBg]               = ImVec4(0.03f, 0.03f, 0.03f, 0.94f);
      colors[ImGuiCol_ChildBg]                = ImVec4(0.06f, 0.06f, 0.06f, 0.00f);
      colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
      colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
      colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      colors[ImGuiCol_FrameBg]                = ImVec4(0.12f, 0.13f, 0.13f, 0.54f);
      colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.21f, 0.25f, 0.29f, 0.40f);
      colors[ImGuiCol_FrameBgActive]          = ImVec4(0.39f, 0.41f, 0.44f, 0.67f);
      colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
      colors[ImGuiCol_TitleBgActive]          = ImVec4(0.23f, 0.24f, 0.27f, 1.00f);
      colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
      colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
      colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
      colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
      colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
      colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
      colors[ImGuiCol_CheckMark]              = ImVec4(0.43f, 0.46f, 0.50f, 1.00f);
      colors[ImGuiCol_SliderGrab]             = ImVec4(0.48f, 0.51f, 0.54f, 1.00f);
      colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.52f, 0.55f, 0.59f, 1.00f);
      colors[ImGuiCol_Button]                 = ImVec4(0.42f, 0.47f, 0.52f, 0.40f);
      colors[ImGuiCol_ButtonHovered]          = ImVec4(0.27f, 0.28f, 0.30f, 1.00f);
      colors[ImGuiCol_ButtonActive]           = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
      colors[ImGuiCol_Header]                 = ImVec4(0.48f, 0.51f, 0.54f, 0.31f);
      colors[ImGuiCol_HeaderHovered]          = ImVec4(0.29f, 0.31f, 0.33f, 0.80f);
      colors[ImGuiCol_HeaderActive]           = ImVec4(0.17f, 0.17f, 0.18f, 1.00f);
      colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
      colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.33f, 0.35f, 0.37f, 0.78f);
      colors[ImGuiCol_SeparatorActive]        = ImVec4(0.27f, 0.29f, 0.31f, 1.00f);
      colors[ImGuiCol_ResizeGrip]             = ImVec4(0.32f, 0.33f, 0.35f, 0.20f);
      colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.29f, 0.32f, 0.34f, 0.67f);
      colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.41f, 0.43f, 0.46f, 0.95f);
      colors[ImGuiCol_TabActive]              = ImVec4(0.32f, 0.35f, 0.40f, 0.86f);
      colors[ImGuiCol_TabHovered]             = ImVec4(0.21f, 0.23f, 0.24f, 0.80f);
      colors[ImGuiCol_Tab]                    = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);
      colors[ImGuiCol_TabUnfocused]           = ImVec4(0.14f, 0.15f, 0.17f, 0.97f);
      colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.21f, 0.23f, 0.25f, 1.00f);
      colors[ImGuiCol_DockingPreview]         = ImVec4(0.16f, 0.17f, 0.18f, 0.70f);
      colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
      colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
      colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
      colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
      colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
      colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
      colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
      colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
      colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
      colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
      colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
      colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
      colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
      colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
      colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

   }
