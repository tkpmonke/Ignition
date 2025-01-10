#include "window.hpp"
#include "camera.hpp"
#include "project.hpp"
#include "utils/files.hpp"
#include "lua/appinfo.hpp"

#include "vels/vels.hpp"
#include "modules/physics/physics.hpp"
#include "modules/physics/rigidbody.hpp"

#include "modules/rendering/light.hpp"

#include <filesystem>
#include "utils/io.hpp"
#include "input.hpp"

#include "angelscript/angelscript.hpp"

bool Ignition::IO::editor = false;
int main() {
#ifdef EDITOR 
   Ignition::IO::DebugPrint("uh this shouldn't be an editor");
#endif
   std::filesystem::path exePath = Ignition::IO::ReadTextFile("/proc/self/exe");
   Ignition::IO::SetProjectHome(exePath.parent_path().string());


   bool applicationOpen = true;
   while (applicationOpen) {
      Ignition::Window window = Ignition::Window(1920, 1080, exePath.filename().string().data(), &applicationOpen);
      glfwSetScrollCallback((GLFWwindow*)window, Ignition::IO::scrollCallback);

      Ignition::Scripting::Lua::LoadCameraInfo();
      Ignition::Camera camera = Ignition::Camera(&window);
      camera.projectionMode = Ignition::Camera::Perspective;
      camera.transform.position = Ignition::Scripting::Lua::cameraInfo.position;
      camera.transform.rotation = Ignition::Scripting::Lua::cameraInfo.rotation;
      camera.fov = Ignition::Scripting::Lua::cameraInfo.fov;
      camera.clippingPlanes.min = Ignition::Scripting::Lua::cameraInfo.min;
      camera.clippingPlanes.max = Ignition::Scripting::Lua::cameraInfo.max;

      camera.MakeMainCamera();

      Ignition::Physics::physicsWorld = std::make_shared<Ignition::Physics::World>();
      Ignition::Physics::physicsWorld->Init();

      Ignition::project.LoadProject(&window);

      Ignition::Rendering::directionalLight.Init();

      Ignition::Scripting::AngelScript::InitilizeAngelScript();
 
      while (window.IsOpen())
      {
         window.Update();

         Ignition::Rendering::RenderShadowMaps();

         window.Bind();
         camera.BeginRender();
          
         Ignition::scene.Update();

         Ignition::Physics::physicsWorld->Update( [&](){Ignition::scene.FixedUpdate();}, 
                              [&](){Ignition::scene.LateFixedUpdate();});

         camera.EndRender(false);
         
         int w,h;
         glfwGetFramebufferSize(window, &w,&h);
         camera.size.x = w;
         camera.size.y = h;
         glViewport(0, 0, w, h);
         window.Resize(w,h);

      }

      Ignition::scene.Shutdown();
      Ignition::Physics::physicsWorld->Shutdown();
      window.Shutdown();
   }
}
