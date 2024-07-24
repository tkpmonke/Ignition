#pragma once

#include <string>

#define CREATE_MODULE(name) std::string type() override { return name; }
namespace Ignition {
   class Module {
   public:
      virtual void Start() {};
      virtual void Update() {};
      virtual void Shutdown() {};

      virtual std::string type() = 0; 

      bool enabled;
   };
}
