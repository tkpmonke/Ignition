#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <optional>
#include <vector>

#include "io.hpp"

namespace bdff {
   class Ref {
   public:
      std::string name;
      
      // has value
      Ref(std::string name, std::string data) : name(name), data(data) {}
      
      // has no value
      Ref(std::string name) : name(name) {}
   
      // null
      Ref() = default;
   
      operator std::string() const { return data; }
   
      Ref& operator[](std::string s) const {
         for (auto& v : *table_val->get()) {
            if (v.name == s) {
               return v;
            }
         }
   
         Ignition::IO::FatalError(name+" is not a table, please fix that");
   
         auto b = new Ref();
         return *b;
      } 
   
      bool isTable() { return table_val.has_value(); }
   
   private:
      std::string data;
      std::optional<std::unique_ptr<std::vector<Ref>>> table_val;
   };
}
