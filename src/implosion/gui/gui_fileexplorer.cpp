#include "gui/gui.hpp"
#include "utils/files.hpp"

#include <iostream>
#include <filesystem>

namespace Implosion {
   

   bool b = 0;
   void GUI::RefreshFiles() {
      if (files.activeDirectory == "") {
         if (b == 0)
            files.activeDirectory = FS::GetProjectHome();
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
   }

   void GUI::FileExplorer() {
      int b=2;
      
      if (files.files.size() == 0)
         RefreshFiles();

      if (ImGui::Begin("Explorer"))
      {

         if (ImGui::BeginChild("##ExplorerFiles", ImVec2(0, ImGui::GetWindowHeight()-100))) {
            
            if (ImGui::BeginChild("##BACK_OF_EXPLORER_",
                        ImVec2(files.size, files.size+20)))
            {
               if (ImGui::Button("##BUTTON___BACK___", ImVec2(files.size,files.size)))
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

               if (ImGui::BeginChild(("##CHILD_OF_EXPLORER_"+std::to_string(i)).data(),
                        ImVec2(files.size, files.size+25)))
               {
                  int type;
                  if (file.type == 0) type = files.file;
                  if (file.type == 1) type = files.folder;
                  if (file.type == 2) type = files.empty_folder;
                  if (ImGui::ImageButton((void*)(intptr_t)type, ImVec2((int)files.size,(int)files.size)))
                  {
                     switch (file.type) {
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
                     ImGui::Text("Dragging");
                     ImGui::EndDragDropSource();
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
         ImGui::SliderFloat("##ExplorerSize", &files.size, 10, 400);
      }

      

      ImGui::End();
   }
}
