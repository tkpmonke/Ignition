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
      o.id = objects.size()-1;
      objects.push_back(o);
      return objects.size()-1;
   }

   void Scene::WriteSceneToDisk()
   {
      Ignition::IO::BeginBinaryWrite(Ignition::IO::GetProjectHome() + "/" + name + ".igscn"); 
      Ignition::IO::Write32(objects.size());

      for (int i = 0; i < objects.size(); ++i)
      {
         Ignition::IO::WriteString(objects[i].name);
         Ignition::IO::WriteString(objects[i].tag);

         Ignition::IO::WriteFloat(objects[i].transform.position.x);
         Ignition::IO::WriteFloat(objects[i].transform.position.y);
         Ignition::IO::WriteFloat(objects[i].transform.position.z);

         Ignition::IO::WriteFloat(objects[i].transform.rotation.x);
         Ignition::IO::WriteFloat(objects[i].transform.rotation.y);
         Ignition::IO::WriteFloat(objects[i].transform.rotation.z);

         Ignition::IO::WriteFloat(objects[i].transform.scale.x);
         Ignition::IO::WriteFloat(objects[i].transform.scale.y);
         Ignition::IO::WriteFloat(objects[i].transform.scale.z);

         Ignition::IO::Write8(objects[i].enabled);

         Ignition::IO::Write16(objects[i].GetModuleCount());

         for (std::shared_ptr<Module> m : objects[i].GetModules())
         {
            Ignition::IO::WriteString(m->mod_type()); 
            Ignition::IO::Write8(m->enabled);
            m->Serialize();
         }  
      }

      Ignition::IO::EndBinaryWrite();
   }

   void Scene::ReadSceneFromDisk()
   {
      if (!Ignition::IO::BeginBinaryRead(Ignition::IO::GetProjectHome() + "/" + name + ".igscn")) {
         std::cerr << "can't open scene\n";
         return;
      }
      uint32_t count = Ignition::IO::Read32();

      for (int i = 0; i < count; ++i)
      {
         //std::cout << i << "\n";

         Object o;

         o.name = Ignition::IO::ReadString();
         o.tag = Ignition::IO::ReadString();

         o.transform.position.x = Ignition::IO::ReadFloat();
         o.transform.position.y = Ignition::IO::ReadFloat();
         o.transform.position.z = Ignition::IO::ReadFloat();
         o.transform.rotation.x = Ignition::IO::ReadFloat();
         o.transform.rotation.y = Ignition::IO::ReadFloat();
         o.transform.rotation.z = Ignition::IO::ReadFloat();
         o.transform.scale.x = Ignition::IO::ReadFloat();
         o.transform.scale.y = Ignition::IO::ReadFloat();
         o.transform.scale.z = Ignition::IO::ReadFloat();

         o.enabled = Ignition::IO::Read8();

         int mCount = Ignition::IO::Read16();

         for (int m = 0; m < mCount; ++m)
         {
            std::string type = Ignition::IO::ReadString();
            bool enabled = Ignition::IO::Read8();

            if (type == "Mesh Renderer")
            {
               MeshRenderer m;
               m.enabled = enabled;

               m.Deserialize();

               o.AddModule(std::make_shared<MeshRenderer>(m));
            }
         }

         AddObject(o);
      }

      Ignition::IO::EndBinaryRead();
   }

   void Scene::Shutdown() {
      texture_lookup_table.clear();
      model_lookup_table.clear();
      shader_lookup_table.clear();
      objects.clear();
   }

   Scene scene;
}
