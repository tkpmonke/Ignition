#pragma once

#include <string>
#include <vector>
#include <memory>


#include "module.hpp"
#include "types/transform.hpp"
#include "utils/io.hpp"

#ifndef IGNITION_NO_MODULES
#include "modules/rendering/meshrenderer.hpp"
#include "modules/physics/rigidbody.hpp"
#endif

#define UPDATE_OBJECT() this->transform.UpdateVectors();                                  \
                        for (std::shared_ptr<Module> m : modules)  {                      \
                           m->transform = &this->transform;                               \
                           m->object = this;                                              \
                           if (!m->enabled || !this->enabled) continue;                   \
                           if (!Ignition::IO::InEditor() || m->runs_in_editor()) {        \
                           m->Update();                                                   \
                        }}

namespace Ignition {
   class Object {
   public:
      std::string name;
      std::string tag;
      unsigned int id;
      bool enabled = true;

      Transform transform;

      virtual void Update() {
         this->transform.UpdateVectors();
         for (std::shared_ptr<Module> m : modules)  {
            m->transform = &this->transform;
            m->object = this;
            if (!m->enabled || !this->enabled) continue;
            if (!Ignition::IO::InEditor() || m->runs_in_editor()) { 
               m->Update();
            }
         }
      }

      virtual void Shutdown() {
         for (auto& m : modules) {
            if (!Ignition::IO::InEditor() || m->runs_in_editor())  
               m->Shutdown();
         }
      }

      Object* parent;
      
      void AddModule(std::shared_ptr<Module>);
      void RemoveModule(std::string);

      //template<class T>
      //T* AddModule();

      // GetModule with mod_name
      std::shared_ptr<Module> GetModule(std::string);
      
#ifndef IGNITION_NO_MODULES
      Rendering::MeshRenderer* GetMeshRenderer();
      Physics::Rigidbody* GetRigidbody();
#endif
      
      // GetModule with templates
      //template<class T>
      //T* GetModule();

      std::vector<std::shared_ptr<Module>>& GetModules() { return modules; }



      std::vector<Object*>* GetChildren() { return &children; };
      std::vector<Object*> GetChildrenWithModule(std::string);

      void AddChild(Object* o) { this->children.push_back(o); }

      int GetModuleCount() {return modules.size();}

      Transform* GetTransform() {
         return &transform;
      }
      void SetTransform(const Transform* t) { transform = *t; }

      bool operator==(const Object& other) const {
         return id == other.id;
      }

      Object& operator=(const Object& other) {
         if (this != &other) {
            this->id = other.id;
            this->modules.clear();
            this->modules = other.modules;
            this->transform = other.transform;
            this->name = other.name;
         }
         return *this;
      }
      
   private:
      std::vector<std::shared_ptr<Module>> modules;
      std::vector<Object*> children;
   };
}
