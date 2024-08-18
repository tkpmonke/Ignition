#pragma once

#include <string>
#include <memory>

#include "types/transform.hpp"

#define CREATE_MODULE(name) const std::string type() const override { return name; }
namespace Ignition {
   class Module {
   public:
      virtual void Start() {};
      virtual void Update() {};
      virtual void Shutdown() {};
      virtual void Serialize() {};
      virtual void Deserialize() {};

      virtual const std::string type() const = 0; 

      bool enabled;
      Transform* transform;
   };
}
