#include "project.hpp"
#include "utils/files.hpp"

#include <iostream>

namespace Ignition {
   Project project;
   void Project::LoadProject()
   {
      if (FS::BeginBinaryRead(FS::GetProjectHome() + "/project") == false)
      {
         std::cerr << "can't find project file\n";
         return;
      }
      this->name = FS::ReadString();
      int sCount = FS::Read16();
      this->currentScene = FS::Read16();

      for (int i = 0; i < sCount; ++i)
      {
         Scene s;
         s.id = FS::Read16();
         s.name = FS::ReadString(); 
         s.ReadSceneFromDisk();
         scenes.push_back(s);
      }
      
      Ignition::scene = scenes[currentScene];

      FS::EndBinaryRead();
   }
}
