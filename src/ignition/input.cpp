#include "input.hpp"

namespace Ignition::IO {
   float scrollStatusX, scrollStatusY;
   
   void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
      scrollStatusX = xoffset;
      scrollStatusY = yoffset;
   }
   float GetScrollX() {auto x = scrollStatusX; scrollStatusX = 0; return x;}
   float GetScrollY() {auto y = scrollStatusY; scrollStatusY = 0; return y;}

   std::unordered_map<int, bool> keyStates;

   bool GetInput(int key) {
      return glfwGetKey((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_PRESS;
   }

   bool GetInputDown(int key) {
      if (!keyStates.count(key)) {
         keyStates[key] = true;
      }

      bool b = glfwGetKey((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_PRESS;
      if (b && keyStates[key]) {
         keyStates[key] = false;
         return true;
      }
      return false;
   }
   
   bool GetInputUp(int key) {
      if (!keyStates.count(key)) {
         keyStates[key] = true;
      }

      bool b = glfwGetKey((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_RELEASE;
      if (b && keyStates[key]) {
         keyStates[key] = false;
         return true;
      }
      return false;
   }

   bool GetMouse(int key) {
      return glfwGetMouseButton((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_PRESS;
   }

   bool GetMouseDown(int key) {
      if (!keyStates.count(key)) {
         keyStates[key] = true;
      }

      bool b = glfwGetMouseButton((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_PRESS;
      if (b && keyStates[key]) {
         keyStates[key] = false;
         return true;
      }
      return false;
   }
   
   bool GetMouseUp(int key) {
      if (!keyStates.count(key)) {
         keyStates[key] = true;
      }

      bool b = glfwGetMouseButton((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_RELEASE;
      if (b && keyStates[key]) {
         keyStates[key] = false;
         return true;
      }
      return false;
   }
}
