#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <any>

#include "types/transform.hpp"

namespace Ignition {
   class Object;
   class Module {
   public:
      /// Called on start
      virtual void Start() {};
      virtual void Shutdown() {};


      virtual void Update() {};
      virtual void FixedUpdate() {};
      virtual void LateUpdate() {};
      virtual void LateFixedUpdate() {};

      virtual void Serialize() {};
      virtual void Deserialize() {};

      virtual bool _runs_in_editor() const { return false; }
      virtual const std::string _get_name() { return "Module"; }; 

      using Properties = std::unordered_map<std::string, std::any>;
      Properties properties;
      virtual void BindProperties() {}

      bool enabled = true;
      Transform* transform;
      Object* object;
      unsigned int id;
   };
}
