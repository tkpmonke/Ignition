#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <shlobj.h>
#include <locale>
#include <codecvt>
#else
#error your out of luck ig (for now)
#endif

#include <cstdlib>
#include <iostream>
#include <vector>

#include <fstream>
#include <sstream>
#include <filesystem>

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void Style();

bool isInstalled = true, acctuallyInstalled = true;

void clear()
{
#ifdef __unix__
      std::system("clear");
#else
      std::system("cls");
#endif
}

bool checkDependinces()
{
   if (std::system("curl --help") != 0)
   {
      clear(); 
      std::cerr << "Curl needs to be installed to use the hub\n";
      return 0;
   }
   if (std::system("python --version") != 0)
   {
      clear(); 
      std::cerr << "Python needs to be installed to use the hub\n";
      return 0;
   }
   
   if (std::system("7z --help") != 0)
   {
      clear(); 
      std::cerr << "7z needs to be installed to use the hub\n";
      return 0;
   }
   if (std::system("echo --help") != 0)
   {
      clear();
      std::cerr << "Echo needs to be installed to use the hub\n";
      return 0;
   }
   clear(); 
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
ImGuiWindowFlags flags = 
                 ImGuiWindowFlags_NoMove 
               | ImGuiWindowFlags_NoResize 
               | ImGuiWindowFlags_NoCollapse 
               | ImGuiWindowFlags_NoSavedSettings 
               | ImGuiWindowFlags_NoTitleBar;
int g = -1;

const char* pyScript = 
"'import requests\\n"
"def get_releases():\\n"
"    url = \"https://api.github.com/repos/juice2011/Ignition/releases\"\\n"
"    response = requests.get(url)\\n"
"    response.raise_for_status()\\n"
"    releases = response.json()\\n"
"    for release in releases:\\n"
"        print(release.get(\"tag_name\"))\\n"
"get_releases()\\n'";

void Installer()
{
   ImGui::Begin("Installer", NULL, flags);
   {

      ImGui::SetWindowPos(ImVec2(0, 0));
	   ImGui::SetWindowSize(ImVec2(640, 484));
          
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
         std::string s = "curl -L https://github.com/juice2011/Ignition/releases/download/v" + releaseStrs[g] + "/implosion.7z -o /tmp/implosion.7z";
         
         s += " && 7z e /tmp/implosion.7z -o/usr/bin";

         isInstalled = std::system(s.data()) == 0;
         acctuallyInstalled = isInstalled;
      }
      
      if (ImGui::Button("Cancel", ImVec2(625, 0)))
      {
         isInstalled = true;
      }
      ImGui::PopStyleVar(1);

      ImGui::End(); 
   }
}

struct Project {
   std::string path;
   std::string name;
};

std::vector<Project> projects;

#ifdef __linux__
std::string GetHome()
{
   const char* home;
   if ((home = getenv("XDG_CONFIG_HOME")) == NULL)
      if ((home = getenv("HOME")) == NULL)
         home = getpwuid(getuid())->pw_dir;
   return home;
}
#elif defined(_WIN32) || defined(_WIN64)
std::string GetHome()
{
   std::wstringstream ss;
   HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
   if (FAILED(hr)) 
   {
      std::cerr << "Failed to initialize COM library." << std::endl;
   }
    
   PWSTR path = NULL;
   hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);
   if (SUCCEEDED(hr)) 
   {
      std::filesystem::path appDataPath(path);
     
      ss << appDataPath;

      CoTaskMemFree(path);
   }
   else
   {
      std::cerr << "Failed to get AppData path." << std::endl;
   }
      
   CoUninitialize();
   return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(ss.str());
}
#endif 
void GetProjects()
{
   projects.erase(projects.begin(), projects.end());
   if (!std::filesystem::exists(GetHome() + "/Implosion-hub"))
      std::filesystem::create_directory(GetHome()+"/Implosion-hub");

   std::ifstream f(GetHome() + "/Implosion-hub/projects");
   std::string s;

   while (std::getline(f, s))
   {
      Project p;
      p.path = s.substr(0, s.find(':'));
      p.name = s.substr(s.find(':') + 1, s.size());
      projects.push_back(p);
   }

   f.close();
}

int main()
{
   glfwInit();
   glfwWindowHint(GLFW_FLOATING, true);
   glfwWindowHint(GLFW_RESIZABLE, false);
   GLFWwindow* window = glfwCreateWindow(640, 484, "Implosion Hub", NULL, NULL); 
   glfwMakeContextCurrent(window);

   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGui::SetCurrentContext(ImGui::GetCurrentContext());
   ImGuiIO& io = ImGui::GetIO();
   io.IniFilename = NULL;
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 330");

   Style();

   if (!checkDependinces())
      return -1;

   if (std::system("implosion -i") != 0)
   {
      acctuallyInstalled = 0;
      if (getuid()) 
      {
         std::cerr << "Superuser Privlege is required for installer\n";
         return -1;
      }
      std::string h = "echo -e " + (std::string)pyScript + " > /tmp/implosion-temp.py";
      std::system(h.data());
      FILE* scraper = popen("python /tmp/implosion-temp.py", "r");
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
      std::system("rm /tmp/implosion-temp.py");
   }
   
   GetProjects();
   
   int selected = -1;
   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      if (!isInstalled)
      {
         Installer();
      } 
      else {
         if (ImGui::Begin("Hub", NULL, flags))
         {
            ImGui::SetWindowPos(ImVec2(0, 0));
	         ImGui::SetWindowSize(ImVec2(640, 484));
            ImGui::Text("Projects");
            if (ImGui::BeginChild("Projects", ImVec2(500, 440)))
            {
               if (projects.size() == 0) {
                  ImGui::Text("Either Create A New Project Or Add On From Disk.");
               } else {
                  for (int i = 0; i < projects.size(); ++i)
                  {
                     bool isSelected = (selected == i);
                     if (ImGui::Selectable((projects[i].name + '\n' + projects[i].path + '\n').data(), isSelected))
                     {
                        selected = i;
                     }
                  }
               }
               ImGui::EndChild();
            }
            ImGui::SameLine();

            if (ImGui::BeginChild("Options", ImVec2(100,0)))
            {
               if (ImGui::Button(acctuallyInstalled ? "Reinstall" : "Install", ImVec2(100, 84)))
               {
                  isInstalled = false;
               }
               if (ImGui::Button("Create", ImVec2(100, 84)))
               {
                  std::ofstream o;
                  o.open(GetHome() + "/Implosion-hub/projects", std::ios_base::app);
                  o << "path:name\n";
                  projects.push_back({.path="path",.name="name"});
                  o.close();
               }
               if (ImGui::Button("Add Local", ImVec2(100,84)))
               {

               }
               if (selected != -1) {
                  if (ImGui::Button("Run", ImVec2(100, 84)))
                  {

                  }
                  if (ImGui::Button("Remove", ImVec2(100,84)))
                  {
                     std::string h;
                     std::ifstream ifs(GetHome() + "/Implosion-hub/projects");
                     std::vector<std::string> lines;
                     std::string o;

                     int b = -1;
                     
                     while (std::getline(ifs, h))
                     {
                        b++; 
                        if (b != selected)
                           o += h + "\n";
                     }
                     ifs.close();
                     std::ofstream ofs(GetHome() + "/Implosion-hub/projects");
                     ofs << o;
                     ofs.close();

                     GetProjects();
                  }
               }
               ImGui::EndChild();
            }
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
      style.ChildBorderSize = 5;
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
      colors[ImGuiCol_ChildBg]                = ImVec4(0.06f, 0.06f, 0.06f, 1.f);
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
      colors[ImGuiCol_HeaderHovered]          = ImVec4(0.29f, 0.31f, 0.33f, 0.84f);
      colors[ImGuiCol_HeaderActive]           = ImVec4(0.17f, 0.17f, 0.18f, 1.00f);
      colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
      colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.33f, 0.35f, 0.37f, 0.78f);
      colors[ImGuiCol_SeparatorActive]        = ImVec4(0.27f, 0.29f, 0.31f, 1.00f);
      colors[ImGuiCol_ResizeGrip]             = ImVec4(0.32f, 0.33f, 0.35f, 0.20f);
      colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.29f, 0.32f, 0.34f, 0.67f);
      colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.41f, 0.43f, 0.46f, 0.95f);
      colors[ImGuiCol_TabActive]              = ImVec4(0.32f, 0.35f, 0.40f, 0.86f);
      colors[ImGuiCol_TabHovered]             = ImVec4(0.21f, 0.23f, 0.24f, 0.84f);
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
      colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.84f, 0.80f, 0.80f, 0.20f);
      colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.84f, 0.80f, 0.80f, 0.35f);

   }
