#include "scene.hpp"
#include "utils/files.hpp"

#include "modules/rendering/meshrenderer.hpp"
#include "modules/script.hpp"
#include "modules/physics/rigidbody.hpp"
#include "modules/rendering/light.hpp"

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
   
   void Scene::LateUpdate()
   {
      for (int i = 0; i < objects.size(); ++i)
      {
         for (auto& m : objects[i].GetModules()) 
            m->LateUpdate();
      }
   }
   
   void Scene::FixedUpdate()
   {
      for (int i = 0; i < objects.size(); ++i)
      {
         for (auto& m : objects[i].GetModules()) 
            m->FixedUpdate();
      }
   }
   
   void Scene::LateFixedUpdate()
   {
      for (int i = 0; i < objects.size(); ++i)
      {
         for (auto& m : objects[i].GetModules()) 
            m->LateFixedUpdate();
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
      object.id = objects.size() > 0 ? objects.at(objects.size()-1).id+1 : 0;
      objects.push_back(object);
   }

   int Scene::CreateObject() 
   {
      Ignition::Object o = Ignition::Object();                                                  
      o.enabled = true;
      o.name = "Object " + std::to_string(objects.size());                                     
      o.tag = "Default";                                                      
      o.id = objects.size() > 0 ? objects.at(objects.size()-1).id+1 : 0;
      objects.push_back(o);
      return objects.size()-1;
   }

   void Scene::Render(int program) {
      for (auto& obj : objects) {
         auto m = obj.GetMeshRenderer();

         if (m != nullptr) {
            m->transform = &obj.transform;
            m->object = &obj;
            if (!m->enabled || !obj.enabled) continue;
            
            int i = m->shader.program;
            m->shader.program = program;
            auto t = m->shader.type;
            m->shader.type = ShaderType_None;
            
            m->Update();

            m->shader.type = t;
            m->shader.program = i;
         }
      }

      Rendering::currentVao = std::numeric_limits<int>::max();
      Rendering::currentProgram = std::numeric_limits<int>::max();
   }

   void Scene::CompileScripts() {

   }

   void Scene::WriteSceneToDisk()
   {
      Ignition::IO::BeginBinaryWrite(Ignition::IO::GetProjectHome() + "/" + name + ".igscn"); 
      Ignition::IO::Write32(objects.size());

      for (int i = 0; i < objects.size(); ++i)
      {
         Ignition::IO::WriteString(objects[i].name);
         Ignition::IO::WriteString(objects[i].tag);

         Ignition::IO::WriteVector3(objects[i].transform.position);
         Ignition::IO::WriteVector3(objects[i].transform.rotation);
         Ignition::IO::WriteVector3(objects[i].transform.scale);

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
         Object o;

         o.name = Ignition::IO::ReadString();
         o.tag = Ignition::IO::ReadString();
         //o.id = objects.size() > 0 ? objects.at(objects.size()-1).id+1 : 0;

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
               auto m = std::make_shared<MeshRenderer>();
               m->object = &o;
               m->enabled = enabled;
               m->Deserialize();
               o.AddModule(m);
            }

            if (type == "Script")
            {
               auto m = std::make_shared<Ignition::Script>();
               m->object = &o;
               m->enabled = enabled;
               m->Deserialize();
               o.AddModule(m);
            }

            if (type == "Rigidbody") {
               auto rb = std::make_shared<Ignition::Physics::Rigidbody>();
               rb->object = &o;
               rb->enabled = enabled;
               o.AddModule(rb);
               auto r = (Ignition::Physics::Rigidbody*)o.GetModule("Rigidbody").get();
               r->Deserialize();
            }

            if (type == "Light") {
               auto m = std::make_shared<Ignition::Rendering::Light>();
               m->object = &o;
               m->enabled = enabled;
               m->Deserialize(); 
               o.AddModule(m);
               lights.push_back(m);
            }
         }

         AddObject(o);
      }

      Ignition::IO::EndBinaryRead();
   }

   void Scene::Shutdown() {
      for (auto& o : objects) {
         o.Shutdown();
      }

      texture_lookup_table.clear();
      model_lookup_table.clear();
      shader_lookup_table.clear();
      objects.clear();
   }

   Scene scene;
}
