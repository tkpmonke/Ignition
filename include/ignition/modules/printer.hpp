#include "module_registry.hpp"
#include "utils/io.hpp"

/// this is an example of how to create a module
/// also used for testing core module creation features

class Printer : public Ignition::Module {
   IGMODULE(Printer)
   //IGRUNS_IN_EDITOR() // if you want it to run in the editor
public:
   int value = 0;
   void PrintValue() { Ignition::IO::Print(value); }

   void BindProperties() override {
      properties["PrintValue(string)"] = &Printer::PrintValue;
      properties["value"] = &Printer::value;
   }
};
