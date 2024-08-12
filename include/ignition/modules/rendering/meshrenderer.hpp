#pragma once

#include "types/shader.hpp"
#include "types/model.hpp"
#include "camera.hpp"

#include <GL/glew.h>
#include <module.hpp>

namespace Ignition::Rendering {
   extern int currentProgram;
   extern int currentVao;

   class MeshRenderer : public Module{
   public:
      CREATE_MODULE("Mesh Renderer");

      Shader shader;
      Model model;     

      MeshRenderer() = default;
      MeshRenderer(Camera* camera) : camera(camera) {}      
      
      void LoadShader(Shader s) {this->shader = s; }
      void LoadModel(Model m, std::string name); 
      
      void Update() override;

      void Serialize() override;
      void Deserialize() override;

   private:
      uint vao;
      Camera* camera;

   };

}
