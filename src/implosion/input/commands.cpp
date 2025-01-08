#include "utils/files.hpp"
#include "serialization/saving.hpp"
#include "scene.hpp"
#include "GLFW/glfw3.h"

#include <algorithm>
#ifdef DEBUG 
#include <chrono>
#endif

Implosion::GUI* g;

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
      Ignition::IO::DebugPrint(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + " Milliseconds");
#endif 
   }

   
   if (key == GLFW_KEY_C && mods == GLFW_MOD_CONTROL 
         && action == GLFW_PRESS && g->selectedObject != nullptr) {
      g->copiedObject = *g->selectedObject;
   }

   if (key == GLFW_KEY_V && mods == GLFW_MOD_CONTROL 
         && action == GLFW_PRESS && g->copiedObject.has_value()) {
      Ignition::scene.AddObject(g->copiedObject.value());
   }
   
   if ((key == GLFW_KEY_DELETE)
         && action == GLFW_PRESS
         && g->selectedObject != nullptr)
   {
      auto ptr = Ignition::scene.GetObjects();
      ptr->erase(std::remove(ptr->begin(), ptr->end(), *g->selectedObject), ptr->end());
      g->selectedObject = nullptr;
   }

#ifdef DEBUG
   if ((key == GLFW_KEY_G
         && mods == GLFW_MOD_CONTROL)
         && action == GLFW_PRESS)
   {
      Ignition::IO::DebugPrint(std::to_string(sizeof(*g)));
   }
#endif
}

void SetCommandCallback(GLFWwindow* w, Implosion::GUI* gui)
{
   g = gui;
   glfwSetKeyCallback(w, GLFWKeyCallback);
}
