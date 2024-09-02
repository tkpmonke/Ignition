#include "utils/files.hpp"
#include "GLFW/glfw3.h"
#include "serialization/saving.hpp"
#include "scene.hpp"

#ifdef DEBUG 
#include <iostream>
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
      std::cout << "Saving Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " Milliseconds\n";
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
         && action == GLFW_PRESS)
   {
      Ignition::scene.GetObjects()->erase(Ignition::scene.GetObjects()->begin() + g->selectedObject->id);
      g->selectedObject = nullptr;
   }

   if ((key == GLFW_KEY_L
         && mods == GLFW_MOD_CONTROL)
         && action == GLFW_PRESS)
      g->InitAssetLoader();

   if ((key == GLFW_KEY_G
         && mods == GLFW_MOD_CONTROL)
         && action == GLFW_PRESS)
   {
      std::cout << sizeof(*g) << "\n";
   }
}

void SetCommandCallback(GLFWwindow* w, Implosion::GUI* gui)
{
   g = gui;
   glfwSetKeyCallback(w, GLFWKeyCallback);
}
