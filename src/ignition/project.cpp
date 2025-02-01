#include "project.hpp"
#include "utils/files.hpp"
#include "utils/io.hpp"
#include "project.hpp"
#include "packer.hpp"
#include "lua/appinfo.hpp"

#include <filesystem>

#include "utils/stb_image.h"

namespace Ignition {
   Project project;
   void Project::LoadProject(Window* window)
   {
      if (Ignition::IO::BeginBinaryRead(Ignition::IO::GetProjectHome() + "/project") == false)
      {
         Ignition::IO::Error("Cannot find project file in directory " + Ignition::IO::GetProjectHome());
         Ignition::IO::FatalError("can't find project file, possibly ran from wrong directory?");
         return;
      }
      this->name = Ignition::IO::ReadString();
      int sCount = Ignition::IO::Read16();
      this->currentScene = Ignition::IO::Read16();

      for (int i = 0; i < sCount; ++i)
      {
         Scene s;
         s.id = Ignition::IO::Read16();
         s.name = Ignition::IO::ReadString(); 
         s.ReadSceneFromDisk();
         scenes.push_back(s);
      }
      
      Ignition::scene = scenes[currentScene];

      Ignition::IO::EndBinaryRead();

      // load icon
      std::string s;
      if (std::filesystem::exists(Ignition::IO::GetProjectHome() + "/icon.png"))
         s = Ignition::IO::GetProjectHome() + "/icon.png";
      else if (std::filesystem::exists("/usr/share/ignition/icon.png"))
         s = "/usr/share/ignition/icon.png";
      else
         return;

      GLFWimage image[1];
      image[0].pixels = stbi_load(s.data(), &image[0].width, &image[0].height, 0, 4);
      glfwSetWindowIcon((GLFWwindow*)*window, 1, image);
      stbi_image_free(image[0].pixels);
   }

   std::string Project::Build() {
      Ignition::scene.WriteSceneToDisk();
      Ignition::Scripting::Lua::LoadAppInfo();

      this->name = Ignition::Scripting::Lua::appInfo.appName;

      pack(Ignition::IO::GetProjectHome().data(),(Ignition::IO::GetProjectHome() + "/bin").data(), 1); 
      auto exeLoc = Ignition::IO::GetProjectHome() + "/bin/" + this->name;
      std::filesystem::copy_file("/usr/bin/ignition-runtime", exeLoc);
      auto soLoc = Ignition::IO::GetProjectHome() + "/bin/libignition.so";
      std::filesystem::copy_file("/usr/lib/libignition.so", soLoc);

      return exeLoc;
   }

#define STARTING_GAME   pid == 0
#define CONTINUE        pid > 0

   void Project::Run() {
      Ignition::Scripting::Lua::LoadAppInfo();
      pid_t pid = fork();

      this->name = Ignition::Scripting::Lua::appInfo.appName;

      if (STARTING_GAME) {
         Ignition::IO::Print("Starting " + this->name);
         auto exeLoc = Ignition::IO::GetProjectHome() + "/bin/";

         chdir(exeLoc.data());
         execl((exeLoc + this->name).data(), this->name.data(), nullptr);

         Ignition::IO::FatalError("Failed To Start Game");
      } else if (CONTINUE) {
         return;
      } else {
         Ignition::IO::Error("Fork Failed, Could Not Start Game");
      }
   }
#undef STARTING_GAME
#undef CONTINUE
}
