#pragma once

#include <string>
#include <vector>

#include "module.hpp"
#include "types/transform.hpp"

#define UPDATE_OBJECT() this->transform.UpdateVectors();                \
                        for (Module* m : modules)  {                    \
                           m->Update();                                 \
                        }

namespace Ignition {
   class Object {
   public:
      std::string name;
      std::string tag;

      Transform transform;

      virtual void Update();

      Object* parent;
      
      void AddComponent(Module*);
      Module* GetComponent(std::string);
      std::vector<Module*> GetComponents(std::string);

      std::vector<Object*> GetChildren() { return children; };
      std::vector<Object*> GetChildrenWithModule(std::string);
      
   private:
      std::vector<Module*> modules;
      std::vector<Object*> children;
   };
}
