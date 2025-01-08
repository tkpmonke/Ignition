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
      bool b = glfwGetKey((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_PRESS;
      if (b == keyStates[key]) {
         keyStates[key] = !b;
         return b == true;
      }
      return false;
   }
   
   bool GetInputUp(int key) {
      bool b = glfwGetKey((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_PRESS;
      if (b == keyStates[key]) {
         keyStates[key] = !b;
         return b == false;
      }
      return false;
   }

   bool GetMouse(int key) {
      return glfwGetMouseButton((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_PRESS;
   }

   bool GetMouseDown(int key) {
      bool b = glfwGetMouseButton((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_PRESS;
      if (b == keyStates[key]) {
         keyStates[key] = !b;
         return b == true;
      }
      return false;
   }
   
   bool GetMouseUp(int key) {
      bool b = glfwGetMouseButton((GLFWwindow*)*Ignition::mainCamera->window, key) == GLFW_PRESS;
      if (b == keyStates[key]) {
         keyStates[key] = !b;
         return b == false;
      }
      return false;
   }

   void LockMouse(bool lock) {
      if (lock)
         glfwSetInputMode((GLFWwindow*)*Ignition::mainCamera->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      else
         glfwSetInputMode((GLFWwindow*)*Ignition::mainCamera->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   }

   void HideMouse(bool hide) {
      if (hide)
         glfwSetInputMode((GLFWwindow*)*Ignition::mainCamera->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      else
         glfwSetInputMode((GLFWwindow*)*Ignition::mainCamera->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   }
}
