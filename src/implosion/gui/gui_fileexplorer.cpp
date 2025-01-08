#include "gui/gui.hpp"
#include "utils/files.hpp"
#include "imgui_stdlib.h"

#include <filesystem>
#include <fstream>
#include <algorithm> // for std::sort
#include <cctype>    // for std::tolower

namespace Implosion {
   

   bool b = 0;

   std::string toLowerCase(const std::string &str) {
      std::string result = str;
      std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
      return result;
   }

   void GUI::RefreshFiles() {
      if (files.activeDirectory == "") {
         if (b == 0)
            files.activeDirectory = Ignition::IO::GetProjectHome();
         else 
            files.activeDirectory = "/";
         b = 1;
      }

      files.files.clear();

      for (const auto& file : std::filesystem::directory_iterator(files.activeDirectory))
      { 
         int type = 0;
        if (file.is_directory() && file.path().filename().string()[0] != '.') 
            type = 1;
         if (files.activeDirectory != "/" && file.path().filename().string()[0] != '.') 
            if (std::filesystem::is_empty(file) && type == 1)
             type = 2;
   
         files.files.push_back({file.path().filename().string(), file.path().string(), type});
      }

      std::sort(files.files.begin(), files.files.end(), [](const auto& a, const auto& b) {
            return toLowerCase(a.name) < toLowerCase(b.name);
      });
   }

   void GUI::FileExplorer() {
      int b=2;
      
      if (files.files.size() == 0)
         RefreshFiles();

      if (ImGui::Begin("Explorer"))
      {

         if (ImGui::BeginChild("##ExplorerFiles", ImVec2(0, ImGui::GetWindowHeight()-100))) {
            
            auto& style = ImGui::GetStyle();
            ImVec2 size = ImVec2((files.size+25)+style.FramePadding.x, (files.size+25)+style.FramePadding.y);
            if (ImGui::BeginChild("##BACK_OF_EXPLORER_",
                        size))
            {
               if (ImGui::Button("##BUTTON___BACK___", ImVec2(files.size+style.FramePadding.x*2, files.size+style.FramePadding.y*2)))
               {
                  files.activeDirectory = files.activeDirectory.substr(0, files.activeDirectory.find_last_of('/'));
                  RefreshFiles();
               }
               ImGui::Text("../");
            }
            ImGui::EndChild();
            ImGui::SameLine();
            for (int i = 0; i < files.files.size(); ++i)
            {
               auto& file = files.files[i];

               // skip folder with build cause it's just annoying
               if ((file.type == 1 || file.type == 2) && file.name == "bin")
                  continue;

               // skip project file cause it's also annoying
               if (file.type == 0 && file.name == "project")
                  continue;

               auto& style = ImGui::GetStyle();
               ImVec2 size = ImVec2((files.size+25)+style.FramePadding.x, (files.size+25)+style.FramePadding.y);
               if (ImGui::BeginChild(("##CHILD_OF_EXPLORER_"+std::to_string(i)).data(),
                        size))
               {
                  int type;

                  if (file.type == 0) {
                     type = files.file;
                     auto s = std::filesystem::path(file.path).extension();

                     if (s == ".igscn") {
                        type = files.additionalTextures.igscn;
                     } else if (s == ".lua") {
                        type = files.additionalTextures.lua_script;
                     }
                  }

                  if (file.type == 1) type = files.folder;
                  if (file.type == 2) type = files.empty_folder;

                  if (ImGui::ImageButton((void*)(intptr_t)type, ImVec2((int)files.size,(int)files.size)))
                  {
                     switch (file.type) {
                        default:
                        case(0):
                           Ignition::IO::EditFile(file.path);
                           break;
                        case(1):
                        case(2):
                           files.activeDirectory = file.path;
                           RefreshFiles();
                           break;
                     }
                  }

                  if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                  {
                     ImGui::SetDragDropPayload("__FILE_EXPLORER_", file.path.data(), file.path.size() +1);
                     ImGui::Text("%s", file.path.substr(Ignition::IO::GetProjectHome().size(),file.path.size()).data());
                     ImGui::EndDragDropSource();
                  } 

                 if (ImGui::BeginPopupContextItem(("##CONTEXT_MENU_"+std::to_string(i)).data())) {
                    if (ImGui::MenuItem(((std::string)"Delete " + (file.type == 0 ? "File" : "Folder")).data())) {
                       std::filesystem::remove(file.path);
                       RefreshFiles();
                    }
                    ImGui::EndPopup();
                 }

                  ImGui::Text("%s",(file.name).data());
               }
               ImGui::EndChild();
               
               if (!(b*files.size + ImGui::GetStyle().ItemSpacing.x
                     >= ImGui::GetWindowWidth() /1.2f))
               {
                  b++;
                  ImGui::SameLine();
               } else {
                  b = 1;
               }
            }
         }

         ImGui::EndChild();
         
         if (ImGui::Button("Refresh"))
            RefreshFiles();
         ImGui::SameLine();
         if (ImGui::Button("Go Up")) {
            files.activeDirectory = files.activeDirectory.substr(0, files.activeDirectory.find_last_of('/'));
            RefreshFiles();
         }
         ImGui::SameLine();
         
         static std::string b;
         ImGui::InputText("##input_string", &b);
         ImGui::SameLine();

         if (ImGui::Button("Make File")) {
            if (b == "") {
               std::ofstream file(files.activeDirectory / (std::filesystem::path)"file.txt");
               file.close();
            } else {
               std::ofstream file(files.activeDirectory / (std::filesystem::path)b);
               file.close();
            }

            RefreshFiles();
            b = "";
         }

         ImGui::SameLine();

         if (ImGui::Button("Make Folder")) {
            if (b == "") {
               std::filesystem::create_directory(files.activeDirectory / (std::filesystem::path)"folder");
            } else {
               std::filesystem::create_directory(files.activeDirectory / (std::filesystem::path)b);
            }
            b = "";
            RefreshFiles();
         }

         ImGui::SliderFloat("##ExplorerSize", &files.size, 10, 400);
      }

      

      ImGui::End();
   }
}
