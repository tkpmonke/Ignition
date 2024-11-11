#include "input/camera_movement.hpp"
#include <algorithm>
#include "imgui.h"
double px, py;

float sensitivity = .1f, movespeed = 5.f, shiftMul = 2;
float prtime;

void cameraMovement(Ignition::Window* window, Ignition::Camera* camera)
{
   double x, y, offx, offy;
   
   glfwGetCursorPos((GLFWwindow*)*window, &x, &y);

   float time = glfwGetTime();
   float dt = time-prtime;
   prtime = time;

   offx = x - px;
   offy = py - y;

   px = x;
   py = y;

   if (glfwRawMouseMotionSupported())
      glfwSetInputMode((GLFWwindow*)*window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

   if (glfwGetMouseButton((GLFWwindow*)*window, GLFW_MOUSE_BUTTON_RIGHT)) {
      //glfwSetInputMode((GLFWwindow*)*window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      glfwSetInputMode((GLFWwindow*)*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      camera->transform.rotation.z += offx * sensitivity;
      camera->transform.rotation.y += offy * sensitivity;

      if (glfwGetKey((GLFWwindow*)*window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
         dt *= shiftMul;

      if (glfwGetKey((GLFWwindow*)*window, GLFW_KEY_W) == GLFW_PRESS)
         camera->transform.position += movespeed * camera->transform.forward * dt;
      if (glfwGetKey((GLFWwindow*)*window, GLFW_KEY_A) == GLFW_PRESS)
         camera->transform.position += movespeed * -camera->transform.right * dt;
      if (glfwGetKey((GLFWwindow*)*window, GLFW_KEY_S) == GLFW_PRESS)
         camera->transform.position += movespeed * -camera->transform.forward * dt;
      if (glfwGetKey((GLFWwindow*)*window, GLFW_KEY_D) == GLFW_PRESS)
         camera->transform.position += movespeed * camera->transform.right * dt;
      if (glfwGetKey((GLFWwindow*)*window, GLFW_KEY_E) == GLFW_PRESS)
         camera->transform.position += movespeed * camera->transform.up * ((float)dt/1.5f);
      if (glfwGetKey((GLFWwindow*)*window, GLFW_KEY_Q) == GLFW_PRESS)
         camera->transform.position += movespeed * -camera->transform.up * ((float)dt/1.5f);

   } else {
      glfwSetInputMode((GLFWwindow*)*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   }


   camera->transform.rotation.y = std::clamp(camera->transform.rotation.y, -89.f, 89.f);
}
