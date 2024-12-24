#pragma once

#include <string>
#include <memory>

#include "types/transform.hpp"

#define CREATE_MODULE(name) const std::string mod_type() override { return name; }
namespace Ignition {
   class Object;
   class Module {
   public:
      virtual void Start() {};
      virtual void Shutdown() {};


      virtual void Update() {};
      virtual void FixedUpdate() {};
      virtual void LateUpdate() {};
      virtual void LateFixedUpdate() {};

      virtual void Serialize() {};
      virtual void Deserialize() {};

      virtual bool runs_in_editor() { return false; }
      virtual const std::string mod_type() { return "Module"; }; 

      bool enabled;
      Transform* transform;
      Object* object;
   };
}
