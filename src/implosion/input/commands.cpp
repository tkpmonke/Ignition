#include "utils/files.hpp"
#include "GLFW/glfw3.h"
#include "serialization/saving.hpp"

#include <iostream>
#include <filesystem>

void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_S && mods == GLFW_MOD_CONTROL 
         && action == GLFW_PRESS) {
      WritePreferences();
   }
}

void SetKeyCallback(GLFWwindow* w)
{
   glfwSetKeyCallback(w, GLFWKeyCallback);
}
