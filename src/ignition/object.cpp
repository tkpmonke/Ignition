#include "object.hpp"

namespace Ignition {

   /*void Object::Update()
   {
      UPDATE_OBJECT();
   }*/

   void Object::AddComponent(std::shared_ptr<Module> mod)
   {
      this->modules.push_back(mod);
   }
}
