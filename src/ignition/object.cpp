#include "object.hpp"

namespace Ignition {

   void Object::Update()
   {

      UPDATE_OBJECT();
   }

   void Object::AddComponent(std::shared_ptr<Module> mod)
   {
      this->modules.push_back(mod);
      mod->transform = std::make_shared<Transform>(this->transform);
      mod->Start();
   }
}
