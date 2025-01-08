#define IGNITION_NO_MODULES

#include "camera.hpp"
#include "window.hpp"

#include <memory>

extern "C" {
   #include "gui.h"
}

std::unique_ptr<Ignition::Window> igWindow;
std::unique_ptr<Ignition::Camera> igCamera;
GLFWwindow* window;

bool Ignition::IO::editor = false;

void initilize_window() {
   bool open = true;
   igWindow = std::make_unique<Ignition::Window>(640, 640, "Implosion Hub", &open);
   igCamera = std::make_unique<Ignition::Camera>(igWindow.get());
   igCamera->projectionMode = Ignition::Camera::Orthographic;
   igCamera->MakeMainCamera();

   window = (GLFWwindow*)*igWindow.get();
}

int update_window() {
   if (igWindow->IsOpen()) {
      igWindow->Update();
      igWindow->Bind();
      igCamera->BeginRender();
      return true;
   }
   return false;
}

void present_window() {
   igCamera->EndRender(false);
   
   int w,h;
   glfwGetFramebufferSize(window, &w,&h);
   igCamera->size.x = w;
   igCamera->size.y = h;
   glViewport(0, 0, w, h);
   igWindow->Resize(w,h);
}

void shutdown_window() {
   igWindow->Close();
}
