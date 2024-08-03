#include "utils/files.hpp"
#include "GLFW/glfw3.h"

#include <iostream>
#include <filesystem>

void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_S && action == GLFW_PRESS)
   {
      std::cout << "Writing Save File\n";
      if (!std::filesystem::exists(FS::GetHome() + "/Ignition"))
         std::filesystem::create_directory(FS::GetHome() + "/Ignition");
      FS::WriteFileFromHome("/Ignition/preferences", "tesf");

      FS::BeginBinaryRead(FS::GetHome() + "/Ignition/preferences");
      uint32_t d = FS::Read32();
      FS::EndBinaryRead();
      std::cout << (char)(d >> 24);
      std::cout << (char)(d >> 16);
      std::cout << (char)(d >> 8);
      std::cout << (char)(d) << "\n";
   }
}

void SetKeyCallback(GLFWwindow* w)
{
   glfwSetKeyCallback(w, GLFWKeyCallback);
}
