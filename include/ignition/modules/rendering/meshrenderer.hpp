#pragma once

#include "types/shader.hpp"
#include "types/model.hpp"

#include <GL/glew.h>
#include "module_registry.hpp"

#include <unordered_map>

namespace Ignition::Rendering {
   extern int currentProgram;
   extern int currentVao;
   extern std::unordered_map<std::string, int> model_lookup_table;

   class MeshRenderer : public Module {
      IGMODULE(MeshRenderer)
      IGRUNS_IN_EDITOR()
   public: 

      Shader shader;
      Model model;     

      /// Set s to currect shader
      void LoadShader(Shader s) {this->shader = s; }

      /// Set and load m to current model
      void LoadModel(Model m); 

      void Start() override;
      
      void Update() override;

      void Serialize() override;
      void Deserialize() override;

      uint vao;
   };

}
