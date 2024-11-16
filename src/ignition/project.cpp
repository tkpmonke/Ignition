#include "project.hpp"
#include "utils/files.hpp"
#include "utils/io.hpp"

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
}
