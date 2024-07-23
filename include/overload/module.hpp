#pragma once
#include <string>

#define CREATE_MODULE(name) std::string type() override { return name; }
namespace Overload {
   class Module {
   public:
      virtual void Update() = 0;
      virtual std::string type() = 0; 
   };
}
