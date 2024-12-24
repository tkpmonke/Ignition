#include "object.hpp"
#include <algorithm>

namespace Ignition {

   void Object::AddModule(std::shared_ptr<Module> mod)
   {
      this->modules.push_back(mod);
      mod->transform = &this->transform;
      if (!Ignition::IO::InEditor() || mod->runs_in_editor())
         mod->Start();
   }

   void Object::RemoveModule(std::string s) {
      std::shared_ptr<Module> mod = nullptr;
      for (auto i : this->modules) {
         if (i->mod_type() == s)
            mod = i;
      }

      if (mod != nullptr) {
         modules.erase(std::remove(modules.begin(), modules.end(), mod), modules.end());
      }
   }

   /*template <class T>
   T* Object::AddModule() {
      T mod = new T();
      auto sp = std::make_shared<T>(mod);
      this->modules.push_back(mod);

      return this->modules.at(modules.size()-1);
   }*/

   std::shared_ptr<Module> Object::GetModule(std::string mod)
   {
      for (auto i : this->modules)
      {
         if (i->mod_type() == mod)
            return i;
      }

      return nullptr;
   }

   /*template<class T>
   T* Object::GetModule()
   {
      for (auto i : this->modules)
      {
         if (typeid(i) == typeid(T))
            return i.get();
      }

      return nullptr;
   }*/

   std::vector<Object*> Object::GetChildrenWithModule(std::string m)
   {
      std::vector<Object*> objs;
      for (int i = 0; i < this->children.size(); ++i)
      {
         if (this->children[i]->GetModule(m))
            objs.push_back(this->children[i]);
      }
      return objs;
   }
}
