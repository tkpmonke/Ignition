#include "gui/gui.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.hpp"

#ifdef __unix__
#  ifdef __linux__
const char* os = "Linux";
#  else
const char* os = "Unix";
#  endif
#elif defined(__APPLE__)
const char* os = "MacOS";
#elif defined(WIN32) || defined(WIN64)
const char* os = "Windows";
#else
const char* os = "Unknown";
#endif

float ptime;
int frameCount = 0;
float fps = 0;
namespace Implosion {
   void GUI::DebugMenu()
   {
ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground;
   if (ImGui::Begin("Debug Menu")) {
      float time = glfwGetTime();
      float dt = time-ptime;
      frameCount++;

      if (dt >= 1.0)
      {
         fps = frameCount;
         frameCount = 0;
         ptime = time;

      }
      

      ImGui::Text("Delta Time %f", dt);
      ImGui::Text("FPS %f", fps);

      ImGui::Separator();

      ImGui::Text("Objects %lu", Ignition::scene.GetObjects()->size());
      ImGui::Text("Objects with tag \"Bananna\" %lu", Ignition::scene.GetObjectsWithTag("Banana").size());

      if (ImGui::CollapsingHeader("Hardware"))
      {
         ImGui::SeparatorText("GPU Info");
         ImGui::Text("GPU Vendor : %s", glGetString(GL_VENDOR));
         ImGui::Text("GPU : %s", glGetString(GL_RENDERER));
         
      }
      if (ImGui::CollapsingHeader("Software"))
      {
         ImGui::SeparatorText("Librarys");
         
         ImGui::Text("OpenGL Version : %s", glGetString(GL_VERSION));
         ImGui::Text("GLFW Version : %s", glfwGetVersionString());
         ImGui::Text("Operating System : %s", os);
         ImGui::Text("Compiled With GCC Version : C = %d, C++ = %d", __GNUC__, __GNUG__);
         
         /*ImGui::SeparatorText("Window");
         
         ImGui::Text("Window Width : %i", (int)renderer->window.width);
         ImGui::Text("Window Height : %i", (int)renderer->window.height);
         */
         ImGui::SeparatorText("Framebuffer");
         int ret;
         glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &ret);
         ImGui::Text("Red Bit Size : %d", ret);
         glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &ret);
         ImGui::Text("Green Bit Size : %d", ret);
         glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &ret);
         ImGui::Text("Blue Bit Size : %d", ret);
         glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &ret);
         ImGui::Text("Alpha Bit Size : %d", ret);
         glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &ret);
         ImGui::Text("Depth Bit Size : %d", ret);
         glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &ret);
         ImGui::Text("Stencil Bit Size : %d", ret);
      }
      /*if (ImGui::CollapsingHeader("Scene"))
      {
         ImGui::Text("Object Count : %d", (int)renderer->objects.size());
         ImGui::Text("Shader Count : %d", (int)renderer->shaders.size());
         ImGui::Text("Camera Position : %f, %f, %f", camera->position.x, camera->position.y, camera->position.z);

      }*/
   }
   ImGui::End();
      
   }
}
