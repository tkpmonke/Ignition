#pragma once

#include <string>
#include <vector>
#include <memory>

#include "module.hpp"
#include "types/transform.hpp"

#define UPDATE_OBJECT() this->transform.UpdateVectors();                \
                        for (std::shared_ptr<Module> m : modules)  {    \
                           m->Update();                                 \
                        }

namespace Ignition {
   class Object {
   public:
      std::string name;
      std::string tag;

      Transform transform;

      virtual void Update() { UPDATE_OBJECT() };

      Object* parent;
      
      void AddComponent(std::shared_ptr<Module>);
     Module* GetComponent(std::string);
      std::vector<Module*> GetComponents(std::string);

      std::vector<Object*> GetChildren() { return children; };
      std::vector<Object*> GetChildrenWithModule(std::string);
      
   private:
      std::vector<std::shared_ptr<Module>> modules;
      std::vector<Object*> children;
   };
}
