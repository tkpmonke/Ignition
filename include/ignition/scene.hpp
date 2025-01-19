#pragma once

#include "object.hpp"
#include "skybox.hpp"

namespace Ignition {

   

   class Scene {
   public:
      void Update();
      void LateUpdate();

      void FixedUpdate();
      void LateFixedUpdate();

      std::vector<Object>* GetObjects() {return &this->objects;}
      std::vector<Object*> GetObjectsWithTag(std::string tag);

      void AddObject(Object object);
      int CreateObject();

      /// used in shadow calculations, not intended for user code
      void Render(int program);

      void CompileScripts();

      void WriteSceneToDisk();
      void ReadSceneFromDisk();

      void Shutdown();

      std::string name;
      int id;

      Skybox skybox;

   private:
      std::vector<Object> objects;
   };
   extern Scene scene;
}
