#include "scene.hpp"

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
   }

   int Scene::CreateObject() 
   {
      Ignition::Object o = Ignition::Object();                                                  
      o.name = "Object " + std::to_string(objects.size());                                     
      o.tag = "Default";                                                      
      objects.push_back(o);
      return objects.size()-1;
   }

   Scene scene;
}
