#pragma once

#include <string>
#include <memory>

#include "types/transform.hpp"

#define CREATE_MODULE(name) const std::string mod_type() const override { return name; }
namespace Ignition {
   class Object;
   class Module {
   public:
      virtual void Start() {};
      virtual void Update() {};
      virtual void Shutdown() {};
      virtual void Serialize() {};
      virtual void Deserialize() {};

      virtual bool runs_in_editor() { return false; }
      virtual const std::string mod_type() const = 0; 

      bool enabled;
      Transform* transform;
      Object* object;
   };
}
