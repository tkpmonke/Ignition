#include "gui/gui.hpp"
#include "packer.hpp"
#include "lua/appinfo.hpp"

#include <filesystem>

#if __has_include("zip.h")
#define ZIP_H_INCLUDED
#include <zip.h>
#endif

namespace Implosion {
   void GUI::MenuBar()
   {
      if (ImGui::BeginMainMenuBar()) {
         if (ImGui::BeginMenu("Implosion"))
         {
            if (ImGui::MenuItem("Exit"))
            {
               exit(0);
            }
            ImGui::EndMenu();
         }
         if (ImGui::BeginMenu("Scene"))
         {
            ImGui::SeparatorText("Add Object");
            AddObjectMenu(); 

            ImGui::EndMenu();
         }
         if (ImGui::MenuItem("Build")) {

            Ignition::Scripting::Lua::LoadAppInfo();

            pack(Ignition::IO::GetProjectHome().data(),(Ignition::IO::GetProjectHome() + "/bin").data(), 1); 
            std::filesystem::copy_file("/usr/bin/ignition-runtime", Ignition::IO::GetProjectHome() + "/bin/"
                  + Ignition::Scripting::Lua::appInfo.appName);
            
#ifdef ZIP_H_INCLUDED
            auto zf = zip_open((Ignition::IO::GetProjectHome()+"/bin/" 
                     + Ignition::Scripting::Lua::appInfo.appName + ".zip").data(), ZIP_CREATE, NULL);
            if (zf != NULL) {
               auto f = zip_dir_add(zf, (Ignition::IO::GetProjectHome()+"/bin").data(), 0);
            }
            zip_close(zf);
#endif
         }
         ImGui::EndMainMenuBar();
      }


      if (openAssetLoader == 1) {
         openAssetLoader = 0;
         this->InitAssetLoader();
      }
   }
}  
