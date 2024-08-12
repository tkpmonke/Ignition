#include "utils/files.hpp"
#include "GLFW/glfw3.h"
#include "serialization/saving.hpp"
#include "scene.hpp"

#ifdef DEBUG 
#include <iostream>
#include <chrono>
#endif
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_S && mods == GLFW_MOD_CONTROL 
         && action == GLFW_PRESS) {
#ifdef DEBUG
      auto start = std::chrono::high_resolution_clock::now();  
#endif 
      WritePreferences();
      Ignition::scene.WriteSceneToDisk();
#ifdef DEBUG
      auto end = std::chrono::high_resolution_clock::now();  
      std::cout << "Saving Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " Milliseconds\n";
#endif 
   }

   if (key == GLFW_KEY_R && mods == GLFW_MOD_CONTROL 
         && action == GLFW_PRESS) {
      Ignition::scene.GetObjects()->clear();
      Ignition::scene.ReadSceneFromDisk();
   }
}

void SetCommandCallback(GLFWwindow* w)
{
   glfwSetKeyCallback(w, GLFWKeyCallback);
}
