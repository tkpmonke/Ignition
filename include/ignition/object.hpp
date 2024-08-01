#pragma once

#include <string>
#include <vector>
#include <memory>

#include "module.hpp"
#include "types/transform.hpp"

#define UPDATE_OBJECT() this->transform.UpdateVectors();                                  \
                        for (std::shared_ptr<Module> m : modules)  {                      \
                           m->transform = std::make_shared<Transform>(this->transform);   \
                           m->Update();                                                   \
                        }

namespace Ignition {
   class Object {
   public:
      std::string name;
      std::string tag;
      bool enabled = true;

      Transform transform;

      virtual void Update();

      Object* parent;
      
      void AddModule(std::shared_ptr<Module>);
     Module* GetModule(std::string);
      std::vector<Module*> GetModules(std::string);

      std::vector<Object*> GetChildren() { return children; };
      std::vector<Object*> GetChildrenWithModule(std::string);
      
   private:
      std::vector<std::shared_ptr<Module>> modules;
      std::vector<Object*> children;
   };
}
