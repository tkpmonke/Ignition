#pragma once

#include <string>
#include <vector>
#include <memory>

#include "module.hpp"
#include "types/transform.hpp"

#define UPDATE_OBJECT() this->transform.UpdateVectors();                                  \
                        for (std::shared_ptr<Module> m : modules)  {                      \
                           m->transform = &this->transform;                               \
                           if (!m->enabled || !this->enabled) continue;                    \
                           m->Update();                                                   \
                        }

namespace Ignition {
   class Object {
   public:
      std::string name;
      std::string tag;
      unsigned int id;
      bool enabled = true;

      Transform transform;

      virtual void Update() {UPDATE_OBJECT();}

      Object* parent;
      
      void AddModule(std::shared_ptr<Module>);

      // GetModule with mod_name
      Module* GetModule(std::string);
      
      // GetModule with templates
      template<class T>
      T* GetModule();

      std::vector<std::shared_ptr<Module>> GetModules() { return modules; }

      std::vector<Object*> GetChildren() { return children; };
      std::vector<Object*> GetChildrenWithModule(std::string);

      void AddChild(Object* o) { this->children.push_back(o); }

      int GetModuleCount() {return modules.size();}
      
   private:
      std::vector<std::shared_ptr<Module>> modules;
      std::vector<Object*> children;
   };
}
