#pragma once

#include "object.hpp"
#include "skybox.hpp"

namespace Ignition {

   

   class Scene {
   public:
      void Update();

      std::vector<Object>* GetObjects() {return &this->objects;}
      std::vector<Object*> GetObjectsWithTag(std::string tag);

      void AddObject(Object object);
      int CreateObject();

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
