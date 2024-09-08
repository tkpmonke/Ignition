#pragma once

#include "types/shader.hpp"
#include "types/model.hpp"
#include "camera.hpp"

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

      Shader shader;
      Model model;     

      MeshRenderer() = default;
      MeshRenderer(Camera* camera) : camera(camera) {}      
      
      void LoadShader(Shader s) {this->shader = s; }
      void LoadModel(Model m); 
      
      void Update() override;

      void Serialize() override;
      void Deserialize() override;

   private:
      uint vao;
      Camera* camera;

   };

}
