#include "scene.hpp"
#include "utils/files.hpp"

#include "modules/rendering/meshrenderer.hpp"

#include <iostream>

using namespace Ignition::Rendering;

namespace Ignition {

   void Scene::Update()
   {
      for (int i = 0; i < objects.size(); ++i)
      {
         objects[i].Update();
      }
   }

   std::vector<Object*> Scene::GetObjectsWithTag(std::string tag)
   {
      std::vector<Object*> o;
      
      for (int i = 0; i < objects.size(); ++i)
      {
         if (objects[i].tag == tag)
            o.push_back(&objects[i]);
      }

      return o;
   }

   void Scene::AddObject(Object object)
   {
      objects.push_back(object);
      objects[objects.size()-1].id = objects.size()-1;
   }

   int Scene::CreateObject() 
   {
      Ignition::Object o = Ignition::Object();                                                  
      o.name = "Object " + std::to_string(objects.size());                                     
      o.tag = "Default";                                                      
      objects.push_back(o);
      return objects.size()-1;
   }

   void Scene::WriteSceneToDisk()
   {
      FS::BeginBinaryWrite(FS::GetProjectHome() + "/" + name + ".igscn"); 
      FS::Write32(objects.size());

      for (int i = 0; i < objects.size(); ++i)
      {
         FS::WriteString(objects[i].name);
         FS::WriteString(objects[i].tag);

         FS::WriteFloat(objects[i].transform.position.x);
         FS::WriteFloat(objects[i].transform.position.y);
         FS::WriteFloat(objects[i].transform.position.z);

         FS::WriteFloat(objects[i].transform.rotation.x);
         FS::WriteFloat(objects[i].transform.rotation.y);
         FS::WriteFloat(objects[i].transform.rotation.z);

         FS::WriteFloat(objects[i].transform.scale.x);
         FS::WriteFloat(objects[i].transform.scale.y);
         FS::WriteFloat(objects[i].transform.scale.z);

         FS::Write8(objects[i].enabled);

         FS::Write16(objects[i].GetModuleCount());

         for (std::shared_ptr<Module> m : objects[i].GetModules())
         {
            FS::WriteString(m->type()); 
            m->Serialize();
         }  
      }

      FS::EndBinaryWrite();
   }

   void Scene::ReadSceneFromDisk()
   {
      if (!FS::BeginBinaryRead(FS::GetProjectHome() + "/" + name + ".igscn")) {
         std::cerr << "can't open scene\n";
         return;
      }
      uint32_t count = FS::Read32();

      for (int i = 0; i < count; ++i)
      {
         //std::cout << i << "\n";

         Object o;

         o.name = FS::ReadString();
         o.tag = FS::ReadString();

         o.transform.position.x = FS::ReadFloat();
         o.transform.position.y = FS::ReadFloat();
         o.transform.position.z = FS::ReadFloat();
         o.transform.rotation.x = FS::ReadFloat();
         o.transform.rotation.y = FS::ReadFloat();
         o.transform.rotation.z = FS::ReadFloat();
         o.transform.scale.x = FS::ReadFloat();
         o.transform.scale.y = FS::ReadFloat();
         o.transform.scale.z = FS::ReadFloat();

         o.enabled = FS::Read8();

         int mCount = FS::Read16();

         for (int m = 0; m < mCount; ++m)
         {
            std::string type = FS::ReadString();

            if (type == "Mesh Renderer")
            {
               MeshRenderer m = MeshRenderer(Ignition::MainCamera::camera);

               m.Deserialize();

               o.AddModule(std::make_shared<MeshRenderer>(m));
            }
         }

         AddObject(o);
      }

      FS::EndBinaryRead();
   }

   Scene scene;
}
