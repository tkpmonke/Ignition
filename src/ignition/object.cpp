#include "object.hpp"

namespace Ignition {

   void Object::Update()
   {

      UPDATE_OBJECT();
   }

   void Object::AddModule(std::shared_ptr<Module> mod)
   {
      this->modules.push_back(mod);
      //mod->transform = std::make_shared<Transform>(this->transform);
      mod->Start();
   }

   Module* Object::GetModule(std::string mod)
   {
      for (auto i : this->modules)
      {
         if (i->type() == mod)
            return i.get();
      }

      return nullptr;
   }
}
