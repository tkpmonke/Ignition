#include "gui/gui.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.hpp"


#define LERP(x,y,z) x*(1-z)+y*z

#ifdef __linux__
#include <iostream>
#include <fstream>

#include "utils/str.hpp"
#endif 

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
         ImGui::Text("Objects with tag \"Banana\" %lu", Ignition::scene.GetObjectsWithTag("Banana").size());

         if (ImGui::CollapsingHeader("Hardware"))
         {
            ImGui::SeparatorText("GPU Info");
            ImGui::Text("GPU Vendor :                    %s", glGetString(GL_VENDOR));
            ImGui::Text("GPU :                           %s", glGetString(GL_RENDERER));
#ifdef GL_NVX_gpu_memory_info
            ImGui::Spacing();
            GLint a;
            glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &a);
            ImGui::Text("GPU Total Memory :              %dMB", a/1000);
            glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &a);
            ImGui::Text("GPU Available Memory :          %dMB", a/1000);
#endif
#ifdef __linux__
            ImGui::SeparatorText("CPU Info");
            this->coreCount=0;
            this->cpuSpeed=0;
            std::ifstream cpu("/proc/cpuinfo");
            std::string line;
            while (std::getline(cpu, line))
            {
               if (line.find("model name") != std::string::npos)
                  this->cpuModel = STR::tail(&line,':');
               if (line.find("processor") != std::string::npos)
                  this->coreCount++;
               if (line.find("cpu MHz") != std::string::npos) {
                  std::string s = STR::tail(&line,':');
                  this->cpuSpeed += std::stof(s.substr(1,s.size()));
                  
               }
            }

            cpu.close();

            ImGui::Text("CPU Model :                     %s", cpuModel.data());
            ImGui::Text("CPU Core Count :                %d", coreCount);
            ImGui::Text("CPU Speed :                     %.0fMhz", cpuSpeed/coreCount);
#endif 
            
         }
         if (ImGui::CollapsingHeader("Software"))
         {
            ImGui::SeparatorText("Librarys");
            
            ImGui::Text("OpenGL Version :                %s", glGetString(GL_VERSION));
            ImGui::Text("GLFW Version :                  %s", glfwGetVersionString());
            ImGui::Text("Operating System :              %s", os);
#if defined(__GNUC__) && defined(__GNUG__)
            ImGui::Text("Compiled With GCC Version :     C = %d, C++ = %d", __GNUC__, __GNUG__);
#endif
            ImGui::SeparatorText("Window");
            
            int w,h;
            glfwGetWindowSize((GLFWwindow*)*this->window,&w,&h);

            ImGui::Text("Window Width :                  %i", (int)w);
            ImGui::Text("Window Height :                 %i", (int)h);
            
            ImGui::SeparatorText("Framebuffer");
            
            ImGui::Text("Framebuffer Width :             %i", (int)camera->size.x);
            ImGui::Text("Framebuffer Height :            %i", (int)camera->size.y);

            ImGui::Spacing();

            ImGui::Text("Scene Framebuffer ID :          %d", this->window->framebuffer);
            ImGui::Text("Scene Framebuffer Texture ID :  %d", this->window->color);
            ImGui::Text("Scene Framebuffer Depth ID :    %d", this->window->depth);

            ImGui::Spacing();

            int ret;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &ret);
            ImGui::Text("Red Bit Size :                  %d", ret);
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &ret);
            ImGui::Text("Green Bit Size :                %d", ret);
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &ret);
            ImGui::Text("Blue Bit Size :                 %d", ret);
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &ret);
            ImGui::Text("Alpha Bit Size :                %d", ret);
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &ret);
            ImGui::Text("Depth Bit Size :                %d", ret);
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &ret);
            ImGui::Text("Stencil Bit Size :              %d", ret);
            glBindFramebuffer(GL_FRAMEBUFFER, this->window->color);

            
         }
         if (ImGui::CollapsingHeader("OpenGL Extensions")) {
            GLint numExtensions;
            glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
            for (GLint i = 0; i < numExtensions; i++) {
               const GLubyte* extension = glGetStringi(GL_EXTENSIONS, i);
               ImGui::Text("%s", extension);
            }

         }
         if (ImGui::CollapsingHeader("Scene"))
         {
            ImGui::Text("Object Count :                  %d", (int)Ignition::scene.GetObjects()->size());
            ImGui::Text("Camera Position :               %f, %f, %f", 
                  this->camera->transform.position.x,
                  this->camera->transform.position.y,
                  this->camera->transform.position.z);

         }
   }
   ImGui::End();
      
   }
}
