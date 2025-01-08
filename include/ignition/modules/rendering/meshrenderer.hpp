#pragma once

#include "types/shader.hpp"
#include "types/model.hpp"

#include <GL/glew.h>
#include <module.hpp>

#include <unordered_map>

namespace Ignition::Rendering {
   extern int currentProgram;
   extern int currentVao;
   extern std::unordered_map<std::string, int> model_lookup_table;

   class MeshRenderer : public Module{
   public:
      CREATE_MODULE("Mesh Renderer");
      bool runs_in_editor() override { return true; }

      Shader shader;
      Model model;     

      MeshRenderer() = default;
      
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
